#include <easy_tcp.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace easy_tcp;
using namespace std;
using namespace chrono_literals;

struct MyData {
    int32_t left, right;
    int32_t speed;
} d;

struct : Client {
    ofstream strm;
    int32_t data_count = 0; //TODO: is this gonna overflow??s
    struct reportData {
        float l_speed, r_speed = 0;
        int32_t l_ticks, r_ticks = 0;
        friend std::ostream & operator << (std::ostream & out , const reportData & r) {
            //return out << "huh";
            //return out << "l_speed: " << r.l_speed << ", r_speed: " << r.r_speed << ", l_ticks; " << r.l_ticks << ", r_ticks: " << r.r_ticks;
            return out << r.l_speed << "," << r.r_speed << "," << r.l_ticks << "," << r.r_ticks;
        }
    };
    struct reportData *report = (reportData *)malloc(sizeof( struct reportData));
    int reportSize = sizeof(struct reportData);

    void received_data(char * b, size_t s) override {
        //write into report
        if (s != reportSize) { return; }

        memcpy(report, b, reportSize);
        //send out to virtual serial port that can be read by Arduino Plotter
        data_count += 1;
        strm << data_count << "," << *report << endl;
    }
} c;

int main ( ) {
    //Clear the file
    ofstream temp;
    temp.open("data.csv", std::ofstream::out | std::ofstream::trunc);
    temp.close();

    //Connect to the robot and start writing analytics data out
    c.strm.open("../data.csv", ios::out);
    c.strm << "count,l_speed,r_speed,l_ticks,r_ticks" << endl;

    c.connect("192.168.137.154", 4500);
    //3 cells is 47.5mm
    //10,000 is max speed for both when i use full PWM and no friction
    //8500 is the max speed that allows for overshoot still in controllable range
        //update, the battery was almost dead when i took this measurement so it should
            //be able to be much above this at full batt
    //it seems to go faster when you put in a speed above max, so something must
    //be wrong with the control code that doesn't actually get it to full speed (P2 probs)
    d.speed = 1000; //9000 is comfortable max speed //1000 is bottom speed
    d.left = 20000; //360 -360 //1000 is about 4 inches
    d.right = 20000;
    ((Connection) c).send_data((char *) &d, sizeof(d));
    cout << "sent data" << endl;

    //BIG Qs
    //how to make tick amount on opposite rotation to be equal?
    //how to handle increases in speed changing the tick values on descent

    //with both positive,
    //with left positive, 1260 is a 360 rotation
    //with right positive 1265 is a full rotation, less consistent
/*
    std::this_thread::sleep_for(2s);

    data.left = 660; //360 -360
    data.right = -660;
    ((Connection) c).send_data((char *) &data, sizeof(data));

    std::this_thread::sleep_for(1s);

    data.left = 4000; //360 -360
    data.right = 4000;
    ((Connection) c).send_data((char *) &data, sizeof(data));

    std::this_thread::sleep_for(2s);

    data.left = 660; //360 -360
    data.right = -660;
    ((Connection) c).send_data((char *) &data, sizeof(data));

*/
    this_thread::sleep_for(30s);
    cout << "disconnecting";
    c.strm.close();

    c.disconnect();
}