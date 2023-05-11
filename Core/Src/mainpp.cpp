#include <mainpp.h>

#include "std_msgs/String.h"

#include "string"
extern int b_bool, y_bool, p_bool, h_bool, pauseHanoi, homefree;
extern char hole[4];
extern short int mission_array[5], rotate_state[3], hand_transfer[4];
char handshake[2] = { 0 };
extern int counttry;
extern int tower_step, temp_pub;
extern int finish, disguise, home, steal, putcherry, score_flag[10], unload,
		unload_temp;
void cake_callback(const std_msgs::String &msg) {
	handshake[0] = msg.data[0];
	handshake[1] = msg.data[1];
//	std::string str = msg.data; // string data
	if (msg.data[0] == 'b' || msg.data[0] == 'y' || msg.data[0] == 'p') {
		mission_array[0] = 0;
		if (msg.data[0] == 'b' && b_bool == 1
				&& hole[(size_t) (msg.data[1] - '0')] != msg.data[0]) {
			b_bool = 0;
			for (int i = 0; i < 4; i++) {
				if (hole[i] == 'b') {
					hole[i] = 0;
					break;
				}
			}
		}
		if (msg.data[0] == 'y' && y_bool == 1
				&& hole[(size_t) (msg.data[1] - '0')] != msg.data[0]) {
			y_bool = 0;
			for (int i = 0; i < 4; i++) {
				if (hole[i] == 'y') {
					hole[i] = 0;
					break;
				}
			}
		}
		if (msg.data[0] == 'p' && p_bool == 1
				&& hole[(size_t) (msg.data[1] - '0')] != msg.data[0]) {
			p_bool = 0;
			for (int i = 0; i < 4; i++) {
				if (hole[i] == 'p') {
					hole[i] = 0;
					break;
				}
			}
		}
		hole[(size_t) (msg.data[1] - '0')] = msg.data[0];
		temp_pub = 0;
//		mission_array[(size_t) (msg.data[1] - '0')] = 1;
		if (msg.data[0] == 'b' && b_bool == 0) {
			b_bool = 1;

		}

		if (msg.data[0] == 'y' && y_bool == 0) {
			y_bool = 1;

		}

		if (msg.data[0] == 'p' && p_bool == 0) {
			p_bool = 1;

		}

	}
	else if (msg.data[0] == 'h') {
		if ((msg.data[1] == '0' || msg.data[1] == '3') && h_bool == 0) {
			h_bool = 1;
			mission_array[0] = 0;
			temp_pub = 0;
		}
		if (msg.data[1] == '1' && homefree == 0) {
			pauseHanoi = 1;
		}
		if (msg.data[1] == '2') {
			pauseHanoi = 0;
		}
	}
	else if (msg.data[0] == '0') {
		if (mission_array[(size_t) (msg.data[1] - '0')] == 1) {
			score_flag[mission_array[(size_t) (msg.data[1] - '0')]] = 1;
		}
	}
	else if (msg.data[0] == 'u') {
		unload = 1;
		unload_temp = 0;

	}
	else if (msg.data[0] == 'd') {
		score_flag[4] = 1;
		disguise = 1;

	}
	else if (msg.data[0] == 'f') {
		score_flag[5] = 1;
		finish = 1;
	}
	else if (msg.data[0] == 'A') {
		score_flag[6] = 1;
		home = 1;
	}
	else if (msg.data[0] == 't') {
		score_flag[7] = 1;
		steal++;
	}
	else if (msg.data[0] == 'r') {

		putcherry = 1;
	}

}

// (1) Data you want publish
std_msgs::Int16MultiArray feedback;
std_msgs::Int16MultiArray gate;
std_msgs::Int16MultiArray door;
std_msgs::String check;
ros::NodeHandle nh;
ros::Subscriber<std_msgs::String> sub("mission1", cake_callback);

// (2) Create a publisher
ros::Publisher pub("donefullness1", &feedback);
ros::Publisher pub1("lightgate1", &gate);
ros::Publisher pub2("hand_door1", &door);
ros::Publisher pub3("handshakier1", &check);
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
void pub_the_gate() {
	gate.data_length = 3;
	gate.data = rotate_state;
	pub1.publish(&gate);
}
void pub_the_hand_transfer() {
	door.data_length = 4;
	door.data = hand_transfer;
	pub2.publish(&door);
}
void hand_shake() {
	check.data = handshake;
	pub3.publish(&check);
}
void setup(void) {
	nh.initNode();
	nh.subscribe(sub);

	// (3) Init your publisher with roscore
	nh.advertise(pub);
	nh.advertise(pub1);
	nh.advertise(pub2);
	nh.advertise(pub3);
}
void loop(void) {
	nh.spinOnce();
	counttry++;
}
