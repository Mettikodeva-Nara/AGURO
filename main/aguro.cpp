#include "aguro.h"

bool Aguro::isStarted()
{
    return this->start;
}

void Aguro::stop()
{
    this->start = false;
    motor(0, 0);
    blink_led(1, 100, CRGB(0, 200, 200));
}

void Aguro::_start()
{
    this->start = true;
    this->updateSensor();
}

void Aguro::init(bool debug, Sensor *sensor)
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(Relay, OUTPUT);
    pinMode(PushButton, INPUT);
    digitalWrite(Relay, HIGH);
    s = sensor;
    s->init(debug);
    this->DEBUG = debug;
    s->calibrateLine();
}

void Aguro::centering()
{
    updateSensor();
}

void Aguro::updateSensor()
{
    // Serial.print("*S");
    for (int i = 0; i < 8; i++)
    {
        sensors[i] = s->readlinebool(i);
        if (sensors[i] == 0)
        {
            leds[i] = CRGB(255, 0, 0);
            FastLED.show();
        }
        else
        {
            leds[i] = CRGB(0, 255, 0);
            FastLED.show();
        }
        Serial.print(String(sensors[i]) + ",");
    }
    // Serial.println("#");
}

// moving the robot
void Aguro::followUntil(char type, int speed)
{
    bool found_garis = false;
    int pass_line = 10;
    if (type == TJ)
    {
        for (int i = 0; i < 100; i++)
        {
            updateSensor();
            while (!sensors[0] || !sensors[7])
            {
                traceLine(speed);
                updateSensor();
                if (sensors[0] && sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
            updateSensor();
            while (sensors[0] || sensors[7])
            {
                traceLine(speed);
                updateSensor();
                if (!sensors[0] && !sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < 10; i++)
        {
            updateSensor();
            while (sensors[0] || sensors[7])
            {
                updateSensor();
                motor(speed, speed);
            }
        }
    }

    else if (type == FR)
    {
        for (int i = 0; i < 100; i++)
        {
            updateSensor();
            while (!sensors[6] || !sensors[7])
            {
                traceLine(speed);
                updateSensor();
                if (sensors[6] && sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
            updateSensor();
            while (sensors[6] || sensors[7])
            {
                traceLine(speed);
                updateSensor();
                if (!sensors[6] && !sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < 10; i++)
        {
            updateSensor();
            while (sensors[6] || sensors[7])
            {
                updateSensor();
                motor(speed, speed);
            }
        }
    }
    else if (type == FL)
    {
        for (int i = 0; i < 100; i++)
        {
            updateSensor();
            while (!sensors[0] || !sensors[1])
            {
                traceLine(speed);
                updateSensor();
                if (sensors[0] && sensors[1])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
            updateSensor();
            while (sensors[0] || sensors[1])
            {
                traceLine(speed);
                updateSensor();
                if (!sensors[0] && !sensors[1])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < 10; i++)
        {
            updateSensor();
            while (sensors[0] || sensors[1])
            {
                updateSensor();
                motor(speed, speed);
            }
        }
    }
}

void Aguro::traceLine(int speed)
{

    float dl, dr, out;
    signed char err = 0;
    static signed int P, D, dErr, last_err;
    float Kp = 1.2, Kd = 0;
    int base_speedl = speed;
    int base_speedr = speed;
    int max_speedl = 200;
    int max_speedr = 200;
    bool flag_dir = false;

    updateSensor();
    //    0 1  2  3 4  5  6 7
    if ((sensors[0] && sensors[1] && sensors[2]) || (sensors[7] && sensors[6] && sensors[5]))
    {
        if (sensors[0] && sensors[1] && sensors[2])
        {
            garisKiri = true;
            garisKanan = false;
        }
        else if (sensors[7] && sensors[6] && sensors[5])
        {
            garisKiri = false;
            garisKanan = true;
        }
        line_found = true;
    }
    else if (sensors[3] == 1 && sensors[4] == 1)
    {
        err = 0;
        line_found = true;
    }
    else if (sensors[3] == 1 || sensors[4] == 1)
    {
        if (sensors[3] == 1)
            err = -10;
        else
            err = 10;
        line_found = true;
    }
    else if ((sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1) || (sensors[7] == 1 && sensors[6] == 1 && sensors[5] == 1))
    {
        if (sensors[0] == 1)
            err = -100;
        else
            err = 100;
        line_found = true;
    }

    else if (sensors[0] == 1 || sensors[7] == 1)
    {
        if (sensors[0])
            if (line_found == true)
                err = -80;
            else
                err = 80;
        else if (line_found == true)
            err = 80;
        else
            err = -80;
        line_found = true;
    }

    else if (sensors[1] == 1 || sensors[6] == 1)
    {
        if (sensors[1])
            err = -40;
        else
            err = 40;
        line_found = true;
    }

    else if (sensors[2] == 1 || sensors[5] == 1)
    {
        if (sensors[2])
            err = -20;
        else
            err = 20;
        base_speedl;
        base_speedr;
        line_found = true;
    }
    else
    {
        if (last_err > 0)
            err = -last_err;
        else if (last_err < 0)
            err = last_err;
        else
            err = 0;
        line_found = false;
    }

    dErr = err - last_err;
    if (line_found)
    {
        last_err = err;
    }
    P = Kp * err;
    D = Kd * dErr;
    out = (float)P + (float)D;
    dl = (float)base_speedl + out;
    dr = (float)base_speedr - out;

    if (dl > max_speedl)
        dl = max_speedl;
    else if (dl < -max_speedl)
        dl = -max_speedl;
    if (dr > max_speedr)
        dr = max_speedr;
    else if (dr < -max_speedr)
        dr = -max_speedr;
    motor(dl, dr);
}

void Aguro::stop_motor()
{
    motor(0, 0);
}
void Aguro::right(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        motor(make_safe(speed), -make_safe(speed));
    }
    motor(0, 0);
    centering();
}

void Aguro::left(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        motor(-make_safe(speed), make_safe(speed));
    }
    motor(0, 0);
    centering();
}

void Aguro::mundur(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        motor(-make_safe(speed), -make_safe(speed));
    }
    motor(0, 0);
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (dist < 25)
    {
        dl = 0;
        dr = 0;
    }
    if (dl < 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, -dl);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, dl);
    }
    if (dr < 0)
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, -dr);
    }
    else
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, dr);
    }
}