#ifndef   MAGNET_HPP
#define   MAGNET_HPP
#include<Arduino.h>

class Magnet {
    public:
        int pwm_pin, sensor_pin, dir_pin;
        Magnet(int dir, int pwm, int sensor) {
            pwm_pin = pwm_pin;
            sensor_pin = sensor_pin;
            dir_pin = dir_pin;
        }
        void setup() {
            pinMode(pwm_pin,OUTPUT);
            pinMode(dir_pin,OUTPUT);
        }
        void update() {
            sensor_val = analogRead(sensor_pin);
        }
        bool getIsGenerating() {return isGenerating;}
        void setIsGenerating(bool isGen) {isGenerating = isGen;}
        int getPWM() {return pwm_val;}
        int getSensorVal() { return sensor_val; }
        void handleMagnet(int target) {
            if (isGenerating) {
                pwm_val = pwmfunc(sensor_val,target);
            } else {
                pwm_val = 0;
            }
            pwm_val = constrain(PWM_CORR * pwm_val, -MAX_PWM_VAL, MAX_PWM_VAL);
        }
        void reset() {
            pwm_val = 0;
            error_i = 0;
            error_prev = 0;
        }
        
    private:
        float pwm_val;
        int sensor_val = 0;
        float KP=0.005,KI=0.002,KD=0.00001;
        float PWM_CORR = 0.5;
        float error_i;
        float error_prev;
        int last_updated_time;
        int MAX_PWM_VAL = 150;
        int THRESHOULD_UPPER = 210;
        int range = 100;
        bool isGenerating = false;
        int pwmfunc(int val, int target) {
            // pid 
            float error = target - val;
            error_i += error;
            float error_d = (error - error_prev) / (float)(micros() - last_updated_time) * 1000000.0;
            pwm_val += error * KP + error_i * KI + error_d * KD;
            last_updated_time = micros();
            error_prev = error;
            Serial.println("pwm : " + (String)pwm_val + " error : " + error + " error_i : " + error_i);
            return pwm_val;
        };
};

#endif 