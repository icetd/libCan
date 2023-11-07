#include "log.h"
#include "can.h"
#include <linux/can.h>
#include <unistd.h>

void rx_callback(struct can_frame rx_frame) 
{
	if (rx_frame.can_id == 0x1)
		LOG(INFO, "bytes: Frame from 0x%0X, DLC=%d , data=%s", 
				rx_frame.can_id, rx_frame.can_dlc, rx_frame.data);
}

int main()
{
	Can *can0 = new Can((char *)"can0");
	initLogger(INFO);
	can0->init();
	can0->setOnCanReceiveDataCallback(rx_callback);

	can0->startAutoRead();

	struct can_frame tx_frame;
	tx_frame = {
		.can_id = 0x201,
		.can_dlc = 1,
	};
	tx_frame.data[0] = 0x41;
	for (;;) {
		can0->transmit(&tx_frame);
		sleep(1);
	}
	return 0;
}
