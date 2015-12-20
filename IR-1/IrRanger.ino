
#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;


sensor_msgs::Range range_msg;
ros::Publisher pub_range( "range_data", &range_msg);

const int analog_pins[] = {3,6,7};
unsigned long range_timer=0;
char frameid[] = "/ir_ranger";
int flag=0,distance[3],tdist=0,d=0;

float getRange(const int pin_num)

{
    int data = analogRead(pin_num)/4;
    // if the ADC reading is too low, 
    //   then we are really far away from anything
    if(data < 10)
        return 254;     // max range
    // Magic numbers to get cm
    int distance= 1309/(data-3);
    return distance; //convert to meters
}



void setup()
{
  nh.initNode();
  nh.advertise(pub_range);
  
  range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.01;
  range_msg.min_range = 0.03;
  range_msg.max_range = 0.4;
  
}

void loop()
{
  // publish the range value every 250 milliseconds
  //   since it takes that long for the sensor to stabilize
  flag=0;
  if ( (millis()-range_timer) > 50)
  
  {
    
    
   tdist=100;
   
      for( int j=0;j<2;j++)
   {
     
     
      distance[j]= getRange(analog_pins[j]);
      if(distance[j]<tdist)
          {
           tdist=distance[j];
          } 
   }
   
   //tdist= getRange(analog_pins[0]);
   if(tdist<50)
      {
                range_msg.range=tdist;
                range_msg.header.stamp = nh.now();
                pub_range.publish(&range_msg);
      }
            
    }
    
    range_timer =  millis() + 100;
  
  nh.spinOnce();
}

