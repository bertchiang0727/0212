#include <mainpp.h>

#include "std_msgs/String.h"

#include "string"
extern int b_bool, y_bool, p_bool, h_bool ;
extern char hole[4];
extern short int mission_array[5];
extern int counttry;
extern int tower_step, temp_pub;
void cake_callback(const std_msgs::String &msg) {

//	std::string str = msg.data; // string data
	if (msg.data[0] == 'b' || msg.data[0] == 'y' || msg.data[0] == 'p') {
		mission_array[0] = 0;
		hole[(size_t) (msg.data[1] - '0')] = msg.data[0];
		temp_pub = 0;
//		mission_array[(size_t) (msg.data[1] - '0')] = 1;
		if (msg.data[0] == 'b' && b_bool == 0)
			b_bool = 1;
		if (msg.data[0] == 'y' && y_bool == 0)
			y_bool = 1;
		if (msg.data[0] == 'p' && p_bool == 0)
			p_bool = 1;
	}
	else if (msg.data[0] == 'h' && h_bool == 0) {
		h_bool = 1;
		mission_array[0] = 0;
	}
}

// (1) Data you want publish
std_msgs::Int16MultiArray feedback;

ros::NodeHandle nh;
ros::Subscriber<std_msgs::String> sub("mission0", cake_callback);

// (2) Create a publisher
ros::Publisher pub("donefullness0", &feedback);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	nh.getHardware()->flush();
}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	nh.getHardware()->reset_rbuf();
//}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) {
		HAL_UART_DeInit(&huart2);
		MX_USART2_UART_Init();
		nh.getHardware()->init();
	}
}

void pub_to_ros() {
	// (4) Publish your data
//	feedback.data = (std_msgs::Int16MultiArray::_data_type*) malloc(
//			5 * sizeof(std_msgs::Int16MultiArray::_data_type));
	feedback.data_length = 5;
	feedback.data = mission_array;
//	feedback.data[5] = tower_step;

	pub.publish(&feedback);
}

void setup(void) {
	nh.initNode();
	nh.subscribe(sub);

	// (3) Init your publisher with roscore
	nh.advertise(pub);
}
void loop(void) {
	nh.spinOnce();
	counttry++;
}
