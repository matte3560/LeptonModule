#include "LeptonThread.h"

#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
#define FPS 27;

LeptonThread::LeptonThread() : QThread(),
	m_range_min(-1000),
	m_range_max(1000),
	m_calibration(8000),
	m_recalibrate(false),
	m_lut_id(2)
{
}

LeptonThread::~LeptonThread() {
}

// Public

void LeptonThread::run()
{
	//create the initial image
	myImage = QImage(80, 60, QImage::Format_RGB888);

	//open spi port
	int port = 1;
	if (SpiOpenPort(port) < 0)
		return; // Port failed to open

	while(true) {

		//read data packets from lepton over SPI
		int resets = 0;
		for(int j=0;j<PACKETS_PER_FRAME;j++) {
			//if it's a drop packet, reset j to 0, set to -1 so he'll be at 0 again loop
			read(spi_fd[port], result+sizeof(uint8_t)*PACKET_SIZE*j, sizeof(uint8_t)*PACKET_SIZE);
			int packetNumber = result[j*PACKET_SIZE+1];
			if(packetNumber != j) {
				j = -1;
				resets += 1;
				usleep(1000);
				//Note: we've selected 750 resets as an arbitrary limit, since there should never be 750 "null" packets between two valid transmissions at the current poll rate
				//By polling faster, developers may easily exceed this count, and the down period between frames may then be flagged as a loss of sync
				if(resets == 750) {
					SpiClosePort(port);
					usleep(750000);
					SpiOpenPort(port);
				}
			}
		}
		if(resets >= 30) {
			qDebug() << "done reading, resets: " << resets;
		}

		frameBuffer = (uint16_t *)result;
		
		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			//skip the first 2 uint16_t's of every packet, they're 4 header bytes
			if(i % PACKET_SIZE_UINT16 < 2) {
				continue;
			}
			
			//flip the MSB and LSB at the last second
			int temp = result[i*2];
			result[i*2] = result[i*2+1];
			result[i*2+1] = temp;
		}

		// Check if calibration needs to be updated
		if ( m_recalibrate )
		{
			performCalibration( frameBuffer );
			m_recalibrate = false;
		}

		int max_value = m_calibration + m_range_max;
		int min_value = m_calibration + m_range_min;
		float diff = max_value - min_value;
		float scale = 255/diff;
		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			if(i % PACKET_SIZE_UINT16 < 2) {
				continue;
			}

			// Calculate LUT index based on framebuffer value
			int lut_index;
			if ( frameBuffer[i] > (uint16_t)max_value )
				lut_index = 255;
			else if ( frameBuffer[i] < (uint16_t)min_value )
				lut_index = 0;
			else
				lut_index = (frameBuffer[i] - min_value) * scale;

			// Get color in LUT
			const int *colormap = colormaps[m_lut_id];
			QRgb color = qRgb(colormap[3*lut_index], colormap[3*lut_index+1], colormap[3*lut_index+2]);

			// Set pixel in output image
			int column = (i % PACKET_SIZE_UINT16 ) - 2;
			int row = i / PACKET_SIZE_UINT16;
			myImage.setPixel(column, row, color);
		}

		//lets emit the signal for update
		emit updateImage(myImage);

	}
	
	//finally, close SPI port just bcuz
	SpiClosePort(port);
}


// Private
void LeptonThread::performCalibration(const uint16_t* buffer)
{
	const int num_pixels = 80*60;
	float average = 0;

	// Calculate the average value in the framebuffer and update calibration
	for( int i = 0; i < FRAME_SIZE_UINT16; i++ ) {
		//skip the first 2 uint16_t's of every packet, they're 4 header bytes
		if(i % PACKET_SIZE_UINT16 < 2)
			continue;

		average += (float)buffer[i] / num_pixels;
	}
	m_calibration = average;
}


// Slots

void LeptonThread::performFFC() {
	//perform FFC
	lepton_perform_ffc();
}

void LeptonThread::setParameters(int range_min, int range_max) {
	m_range_min = range_min;
	m_range_max = range_max;
}

void LeptonThread::calibrate()
{
	// Set flag to recalibrate zero temp on next recieved frame
	m_recalibrate = true;
}

void LeptonThread::setLUT(int id)
{
	m_lut_id = id;
}
