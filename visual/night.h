/**
 * @file night.h
 * @author Tinglei Zhu (zhutinglei@gmail.com)
 * @brief Night / Daytime of a ground station
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SODA_VISUAL_NIGHT_H
#define SODA_VISUAL_NIGHT_H
#include <sofa.h>
#include <sofam.h>

#include "pos2rae.h"


/**
 * @brief Check a ground station is at night or not
 * 
 * @param utc: MJD of UTC timescale
 * @param station_lbh: Ground station longitude(deg), latitude(deg) and height(m) 
 * @param dut: Dut (second)
 * @param elevation: Threshold of solar elevation (deg), default is -9 degree
 * @return true: The ground station is at night
 * @return false: The groud station is in the daytime 
 */
bool isNight(double utc, double station_lbh[3], double xp, double yp, double dut, double elevation = -9.0)
{
    /// Get TDB
    ///
    int j;
    double ut11, ut12, tai1, tai2, tt1, tt2, tdb1, tdb2;
    j = iauUtcut1 (DJM0, utc, dut, &ut11, &ut12 );
    j = iauUtctai ( DJM0, utc, &tai1, &tai2 ); // return value: 1 (dubious year), 0 (OK), -1 (unacceptable date)
    j = iauTaitt ( tai1, tai2, &tt1, &tt2 ); // return value: 0 (OK)
    double g;
    g = (357.53 + 0.9856003 * (tt1 + tt2 - DJ00)) * DD2R;
    iauAnp(g);
    tdb1 = tt1;
    tdb2 = tt2 + (0.001658 * sin(g) + 0.000014 * sin(2.0 * g)) / DAYSEC;

    /// Get the position of sun w.r.t. the Earth
    ///
    double pvh[2][3], pvb[2][3];
    iauEpv00(tdb1, tdb2, pvh, pvb);
    double sun_xyz[3];
    for (int i = 0; i < 3; i++)
    {
        sun_xyz[i] = -pvh[0][i] * DAU; 
    }


    /// Calculate Elevation of the Sun
    ///
    double sun_rae[3];
    pos2rae(sun_xyz, station_lbh, utc, xp, yp, dut, sun_rae);
    return sun_rae[2] < elevation;
}
#endif