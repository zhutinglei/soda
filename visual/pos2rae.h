/**
 * @file pos2rae.h
 * @author Tinglei Zhu(zhutinglei@gmail.com)
 * @brief Convert position vector of a space object to range-azimuth-elevation w.r.t. a ground station
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SODA_VISUAL_POS2RAE_H
#define SODA_VISUAL_POS2RAE_H

#include <sofa.h>
#include <sofam.h>

/**
 * @brief 
 * 
 * @param target_xyz: target position vector in GCRS (m)
 * @param station_lbh: station longitude(deg), latitude(deg) and altitude(m)
 * @param utc: MJD in UTC timescale
 * @param xp: Polar motion xp (arcsecond)
 * @param yp: Polar motion yp (arcsecond) 
 * @param dut: Dut (second)
 * @param target_rae[output]: range(m), azimuth(deg) and elevation(deg)
 * @return int: -1 (failure), 0 (OK), 1 (warning)
 */
int pos2rae(double target_xyz[3], double station_lbh[3], double utc, double xp, double yp, double dut, double target_rae[3])
{
    int j[4]; // to record return status of sofa functions.
    double xyz[3];
    /// Transformation from GCRS to ITRF
    ///
    {
        double ut11, ut12, tai1, tai2, tt1, tt2;
        j[0] = iauUtcut1 (DJM0, utc, dut, &ut11, &ut12 ); // // return value: 1 (dubious year), 0 (OK), -1 (unacceptable date)
        j[1] = iauUtctai ( DJM0, utc, &tai1, &tai2 ); // return value: 1 (dubious year), 0 (OK), -1 (unacceptable date)
        j[2] = iauTaitt ( tai1, tai2, &tt1, &tt2 ); // return value: 0 (OK)
        double r[3][3];
        iauIr(r);
        iauC2t06a(tt1, tt2, ut11, ut12, xp * DAS2R, yp * DAS2R, r);
        iauRxp(r, target_xyz, xyz); // xyz is now ITRF 
    }
    /// Transformation from ITRF to station N-E-Z
    ///
    {
        double t[3]; // translation, i.e. ITRF xyz of the station
        double r[3][3]; // rotation
        j[3] = iauGd2gc(WGS84, station_lbh[0] * DD2R, station_lbh[1] * DD2R, station_lbh[2], t); // return 0 (OK), -1 (illegal identifer), -2 (illegal case)
        iauIr(r);
        iauRz(DPI / 2.0 + station_lbh[0] * DD2R, r);
        iauRx(DPI / 2.0 - station_lbh[1] * DD2R, r);
        for (int i = 0; i < 3; i++)
        {
            xyz[i] -= t[i];
        }
        iauRxp(r, xyz, xyz); /// xyz is now NEZ vector w.r.t. the station
    }

    /// NEZ vector to RAE
    ///
    {
        double azimuth, elevation, range;
        iauC2s(xyz, &azimuth, &elevation);
        azimuth = iauAnp(DPI / 2.0 - azimuth) * DR2D; // [0, 360]
        elevation = elevation * DR2D; // [-180, 180]
        range = iauPm(xyz);
        target_rae[0] = range;
        target_rae[1] = azimuth;
        target_rae[2] = elevation;
    }

    /// Get status
    ///
    std::sort(std::begin(j), std::end(j));
    if (j[0] < 0)
    {
        return -1;
    }
    else if (j[3] > 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
#endif
