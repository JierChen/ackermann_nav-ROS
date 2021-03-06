#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <ackermann_nav/ControlMsg.h>
typedef ackermann_nav::ControlMsg myControlMsg;

using namespace message_filters;
using namespace std;

class MultiSubscribeAndPublish
{
public:
  MultiSubscribeAndPublish()
  {
  //Topics you want to publish
  pub_ = n_.advertise<myControlMsg>("/published_from_copy_cat_3_pub", 1000);
  pub2_ = n_.advertise<myControlMsg>("/published_from_copy_cat_3_pub2", 1000);

  //Topics you want to subscribe to
  talker_sub.subscribe(n_, "chatter", 1000);
  talker_2_sub.subscribe(n_, "chatter_2", 1000);
  talker_3_sub.subscribe(n_, "chatter_3", 1000);

  //Synchronize topics, args are Sync(MySyncPolicy(<queue size>), <message_field for topic1>, <message_field for topic2>, etc.)
  sync.reset(new Sync(MySyncPolicy(10), talker_sub, talker_2_sub, talker_3_sub));
  
  //Make call to callback function, args for _1, _2, etc., must match number of topics subscribing to
  sync->registerCallback(boost::bind(&MultiSubscribeAndPublish::callback, this, _1, _2, _3));

  }

  //Callback function gets called when receiving messages from ALL subscribed topics within ApproximateTime queue size
  void callback(const myControlMsg::ConstPtr& talker_info, const myControlMsg::ConstPtr& talker_2_info, const myControlMsg::ConstPtr& talker_3_info)
  {
    //Initialize output message(s)
    myControlMsg output;
    myControlMsg output2;

    //Read in subscribed info to variables
    double talker_info_vel = talker_info->vel_k_in;
    double talker_info_head = talker_info->head_k_in;
    double talker_2_info_vel = talker_2_info->vel_k_in;
    double talker_2_info_head = talker_2_info->head_k_in;
    double talker_3_info_vel = talker_3_info->vel_k_in;
    double talker_3_info_head = talker_3_info->head_k_in;

    /*


    ALGORITHM CODE GOES HERE


    */


    //fill publish message data fields
    output.header.stamp = ros::Time::now();
    output.vel_k_in = talker_info_vel;
    output.head_k_in = talker_info_head;

    output2.header.stamp = ros::Time::now();
    output2.vel_k_in = talker_2_info_vel;
    output2.head_k_in = talker_2_info_head;
    

    //publish output(s)
    pub_.publish(output);    
    pub2_.publish(output2);
  }

private:

  //NodeHandle object for creating publishers, only need one
  ros::NodeHandle n_;

  //Publisher objects, one per topic to publish to
  ros::Publisher pub_;
  ros::Publisher pub2_;

  //Create message_filters so topics can be synchronized, create one per topic to subscribe to
  message_filters::Subscriber<myControlMsg> talker_sub;
  message_filters::Subscriber<myControlMsg> talker_2_sub;
  message_filters::Subscriber<myControlMsg> talker_3_sub;
  

  //Define sync policy as ApproximateTime, one type per topic to subscribe to
  typedef sync_policies::ApproximateTime<myControlMsg, myControlMsg, myControlMsg> MySyncPolicy;
  typedef Synchronizer<MySyncPolicy> Sync;
  boost::shared_ptr<Sync> sync;

};

int main(int argc, char** argv)
{
  //Initiate ROS
  ros::init(argc, argv, "copy_cat_3_pub2");

  //Class object that will subscribe and publish to topics
  MultiSubscribeAndPublish MSAPObject;

  ros::spin();

  return 0;
}
