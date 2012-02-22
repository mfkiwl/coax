#ifndef __COAX_VISION_CONTROL__
#define __COAX_VISION_CONTROL__

#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/String.h>

#include <coax_msgs/CoaxState.h>
#include <coax_msgs/CoaxRawControl.h>
#include <coax_msgs/CoaxReachNavState.h>
#include <coax_msgs/CoaxConfigureComm.h>
#include <coax_msgs/CoaxSetTimeout.h>

//#include <coax_vision/SetControlMode.h>
//#include <coax_vision/CoaxFMdes.h>

struct SymAxis 
{
	unsigned short axis;
	double value;
};

class CompareSymAxis 
{
public:
	bool operator() (SymAxis& A1, SymAxis& A2); 
};

class ImageProc {
	friend class CoaxVisionControl;


public:
  ImageProc(ros::NodeHandle&);
  ~ImageProc();

private:
  void proc(const sensor_msgs::ImageConstPtr& msg);
  image_transport::ImageTransport it_;
  image_transport::Subscriber Sub_Image;
  image_transport::Publisher Pub_Image;
	// For debug message
	ros::Publisher Debug_Msgs;
};



class CoaxVisionControl 
{
	friend class ImageProc;

public:
	CoaxVisionControl(ros::NodeHandle &);
	~CoaxVisionControl();

	bool reachNavState(int des_state, float timeout);
	bool configureComm(int frequency, int contents);
	bool configureControl(unsigned int rollMode, unsigned int pitchMode, unsigned int yawMode, unsigned int altitudeMode);
	bool setTimeout(unsigned int control_timeout_ms, unsigned int watchdog_timeout_ms);
	
	void coaxStateCallback(const coax_msgs::CoaxState::ConstPtr & message);
	void rawControlPublisher(unsigned int rate);



private:
	ros::ServiceClient reach_nav_state;
	ros::ServiceClient configure_comm;
	ros::ServiceClient configure_control;
	ros::ServiceClient set_timeout;
	
	ros::Subscriber coax_state_sub;
	ros::Publisher raw_control_pub;

	bool LOW_POWER_DETECTED;

	bool coax_nav_mode;
	
	double battery_voltage;
	
	double imu_y; // imu yaw
	double imu_r; // imu roll 
	double imu_p; // imu pitch
	double range_al; // range altitude

	double rc_th; // rc throttle
	double rc_y;  // rc yaw
	double rc_r;  // rc roll
	double rc_p;  // rc pitch
	double rc_trim_th; // rc throttle trim
	double rc_trim_y;  // rc yaw trim
	double rc_trim_r;  // rc roll trim
	double rc_trim_p;  // rc pitch trim
	
	double img_th;  // image throttle
	double img_y; 	// image yaw
	double img_r; 	// image roll
	double img_p; 	// image pitch

	double gyro_ch1;
	double gyro_ch2;
	double gyro_ch3;
	double accel_x;
	double accel_y;
	double accel_z; 


};

using namespace std;

#endif
