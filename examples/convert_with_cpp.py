import pandas as pd
import matplotlib.pyplot as plt

# Import the compiled extension
import coordinate_convert as cc_mod

from coordinate_convert import Point, CoordinateConvert

# Load data (same file used in main.py)
file_path = '20030801.xls'
data = pd.read_excel(file_path)

latitudes = data.iloc[:, 4]
longitudes = data.iloc[:, 5]

# Build C++ objects
cc = CoordinateConvert()

pts_lla = [Point(float(lat), float(lon), 0.0) for lat, lon in zip(latitudes, longitudes)]
# Choose datum point as the first point (or average)
datum_lla = pts_lla[0]
# Convert datum to ECEF using C++ wrapper
datum_ecef = cc.BLH2XYZ(datum_lla)

# Convert list of LLA points to NED relative to datum
pts_ned = cc.BLH2NED_vec(pts_lla, datum_ecef)

# Extract X (North), Y (East) - Note: in this C++ code method returns NED; check ordering
x_coords = [p.x for p in pts_ned]
y_coords = [p.y for p in pts_ned]

plt.figure(figsize=(10, 6))
plt.plot(x_coords, y_coords, marker='o', color='b', markersize=2, linestyle='-', label="GPS Trajectory (NED)")
plt.title('GPS Trajectory in NED Coordinates (C++ conversion)')
plt.xlabel('North (m)')
plt.ylabel('East (m)')
plt.grid(True)
plt.legend()
plt.savefig('gps_trajectory_ned_cpp.png', dpi=300)
print('Saved gps_trajectory_ned_cpp.png')
