from os import path, system
import time

folder = "/data/classenl/Mastercode/smDOM24T_Nuke4_dev/output/for_Chris/"

for theta in range(0, 180, 2):
    for phi in range(0, 360, 2):
        system("./build/mdom_scan_angular -e 1 --nh -t " + str(theta) + " -f " + str(phi) + " -n 1000000 -o " + folder + "mDOM_inIce_t" + str(theta) + "_f" + str(phi)+ ".txt")
        time.sleep(0.2)
