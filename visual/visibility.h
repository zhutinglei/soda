/**
 * @file visibility.h
 * @author Tinglei Zhu(zhutinglei@gmail.com)
 * @brief Calculate optical visibility of a space object w.r.t. a ground station
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <sofa.h>
#include <sofam.h>

#include "pos2rae.h"
#include "night.h"
#include "illuminate.h"

/**
 * @brief A space object of optical visible or not
 * 
 * @param target_xyz: target position vector in GCRS (m)
 * @param station_lbh: station longitude(deg), latitude(deg) and altitude(m)
 * @param utc: MJD in UTC timescale
 * @param xp: Polar motion xp (arcsecond)
 * @param yp: Polar motion yp (arcsecond) 
 * @param dut: Dut (second)
 * @param elevation: Minimun observable elevation (deg) 
 * @return int -3 if the space object is below the minimun elevation
 *             -2 if the space object is in Earth shadow, 
 *             -1 if the ground station is at night, 
 *              1 if visible,
 * @note If the space object is below the minimum elevation, it will return -3 regardless other conditions
 */
int isVisible(double target_xyz[3], double station_lbh[3], double utc, double xp, double yp, double dut, double elevation)
{
    double target_rae[3];
    int j = pos2rae(target_xyz, station_lbh, utc, xp, yp, dut, target_rae);
    bool night = isNight(utc, station_lbh, xp, yp, dut, 0.0);
    bool illuminated = isIlluminated(target_xyz, utc, dut);
    if (target_rae[2] < elevation)
    {
        return -3;
    }
    else if (!illuminated)
    {
        return -2;
    }
    else if (!night)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}