#ifndef TEXTTHREAD
#define TEXTTHREAD

#include <ctime>
#include <stdint.h>

#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)

class LeptonThread : public QThread
{
	Q_OBJECT;

	public:
	LeptonThread();
	~LeptonThread();

	void run();

	public slots:
	void performFFC();
	void setParameters(int range_min, int range_max);
	void calibrate();
	void setLUT(int id);

	signals:
	void updateText(QString);
	void updateImage(QImage);

	private:
	void performCalibration(const uint16_t* buffer);

	private:

	QImage myImage;

	uint8_t result[PACKET_SIZE*PACKETS_PER_FRAME];
	uint16_t *frameBuffer;

	int m_range_min, m_range_max;
	int m_calibration;
	int m_lut_id;
	bool m_recalibrate;

};

#endif
