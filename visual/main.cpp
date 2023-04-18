#include <iostream>
#include <iomanip>
#include "pos2rae.h"
#include "night.h"
#include "illuminate.h"

using namespace std;


int main(int argc, char *argv[])
{
    std::cout.setf(std::ios::fixed);
    /// Test pos2rae
    {
        // GCRS
        // 5U UTC 2023 04 17 00 00 00.000000   6067.783  -4274.984   3311.044   3.647118   5.290357   3.249395
        // Simulated Measurement
        // 5U UTC 2023 04 17 00 00 00.000000 90.103575 29.736455 7804   2787353.320456   26.12  120.23   24.14
        // --longitude 101.181666 --latitude  25.52633 --height 2020.000
        double xyz[3] = {6067783.0, -4274984.0, 3311044.0};
        double lbh[3] = {101.181666, 25.52633, 2020.000};
        double rae[3] = {0.0, 0.0, 0.0};
        double utc = 60051;
        double xp = 0.0, yp = 0.0, dut = 0.0;
        pos2rae(xyz, lbh, utc, xp, yp, dut, rae);
        std::cout << "    5U ";
        for (int i = 0; i < 3; i++)
        {
            std::cout << std::setw(16) << rae[i] << " ";
        }
        std::cout << std::endl;
    }
    /// Test isNight
    {
        double utc = 60051;
        double xp = 0.0, yp = 0.0, dut = 0.0;
        double lbh[3] = {101.181666, 25.52633, 2020.000};
        for (int i = 0; i < 144; i++)
        {
            utc += 10.0 / 1440.0;
            bool night = isNight(utc, lbh, xp, yp, dut, 0.0);
            std::cout << setw(16) << std::setprecision(8) << utc << " " << (night ? "N" : "D") << std::endl;
        }
    }
    /// Test isIlluminated
    {
        // 5U Cross Shadow 2023 04 26 20 20 52.966437
        // 5U UTC 2023 04 26 20 20 52.100000    408.832  -6866.604   1609.783 
        // 5U UTC 2023 04 26 20 20 52.200000    409.520  -6866.498   1610.209 
        // 5U UTC 2023 04 26 20 20 52.300000    410.208  -6866.392   1610.635 
        // 5U UTC 2023 04 26 20 20 52.400000    410.896  -6866.286   1611.061 
        // 5U UTC 2023 04 26 20 20 52.500000    411.585  -6866.179   1611.487 
        // 5U UTC 2023 04 26 20 20 52.600000    412.273  -6866.073   1611.912 
        // 5U UTC 2023 04 26 20 20 52.700000    412.961  -6865.966   1612.338 
        // 5U UTC 2023 04 26 20 20 52.800000    413.649  -6865.860   1612.764 
        // 5U UTC 2023 04 26 20 20 52.900000    414.337  -6865.753   1613.190 
        // 5U UTC 2023 04 26 20 20 53.000000    415.025  -6865.647   1613.615
        double utc = 60060.84782407; // 2023-04-26T20:20:51.999660Z
        double xyz[10][3] = {
            {408832., -6866604., 1609783.},
            {409520., -6866498., 1610209.},
            {410208., -6866392., 1610635.},
            {410896., -6866286., 1611061.},
            {411585., -6866179., 1611487.},
            {412273., -6866073., 1611912.},
            {412961., -6865966., 1612338.},
            {413649., -6865860., 1612764.},
            {414337., -6865753., 1613190.},
            {415025., -6865647., 1613615.}
        };
        double dut = 0.0;
        for (int i = 0; i < 10; i++)
        {
            utc += 0.1 / DAYSEC;
            bool illuminated = isIlluminated(xyz[i], utc, dut);
            std::cout << std::setw(4) << i + 1 << " " << (illuminated ? "L" : "S") << std::endl;
        }
    }

    return 0;
}