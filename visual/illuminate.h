/**
 * @file illuminate.h
 * @author Tinglei Zhu (zhutinglei@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SODA_VISUAL_ILLUMINATE_H
#define SODA_VISUAL_ILLUMINATE_H
#include <sofa.h>
#include <sofam.h>

#include "pos2rae.h"

/**
 * @brief A space object is illuminated by the sun or not
 * 
 * @param target_xyz: target position vector in GCRS (m) 
 * @param utc: MJD in UTC timescale
 * @param dut: MJD in UTC timescale
 * @return true: is illuminated
 * @return false: is not illuminated 
 */
bool isIlluminated(double target_xyz[3], double utc, double dut)
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

    /// Get the direction of sun w.r.t. the Earth
    ///
    double pvh[2][3], pvb[2][3];
    iauEpv00(tdb1, tdb2, pvh, pvb);
    double sun_direction[3];
    double length = iauPm(pvh[0]);
    for (int i = 0; i < 3; i++)
    {
        sun_direction[i] = -pvh[0][i] / length;
    }

    /// Check if the target is illumnated
    ///
    double v = iauPdp(sun_direction, target_xyz);
    return (v > 0 || sqrt(iauPdp(target_xyz, target_xyz) - v*v) > 6400000.0);
}

#endif