//
// Created by moger on 24/06/2021.
//

#ifndef MATPLOTLIB_CPP_BATTERY_H
#define MATPLOTLIB_CPP_BATTERY_H


class Battery {
public:
    Battery();
    Battery(float cap, float rate);

    float depletionRate_;

    float getDepletionRate() const;

    void setDepletionRate(float depletionRate);

    float getCapacity() const;

    void setCapacity(float capacity);

    float getLevel() const;

    void setLevel(float level);

    float capacity_;
    float level_ = capacity_;
};


#endif //MATPLOTLIB_CPP_BATTERY_H
