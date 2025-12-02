"""Run with uv tool if available:
	uv venv .venv -p 3.13 --seed
	uv sync
	uv add pybind11 --dev
	uv run python setup.py build_ext --inplace
	uv run ./main.py
"""
import pandas as pd
import matplotlib.pyplot as plt
import coordinate_convert as cc_mod
from coordinate_convert import Point, CoordinateConvert

# 加载GPS数据
file_path = '20030804.xls'  # 请确保使用正确的文件路径
data = pd.read_excel(file_path)

# 提取经纬度数据
latitudes = data.iloc[:,4]  # 纬度
longitudes = data.iloc[:,5]  # 经度
altitudes = data.iloc[:,6]  # 高度

# Use the C++ converter via pybind11
cc = CoordinateConvert()
pts_lla = [Point(float(lat), float(lon), float(0)) for lat, lon, alt in zip(latitudes, longitudes, altitudes)]
# pick a datum — first point
datum_lla = pts_lla[0]
# For accurate conversion, convert the datum to ECEF and use BLH2NED (BLH2NED_vec) which expects an ECEF datum
datum_ecef = cc.BLH2XYZ(datum_lla)
pts_local = cc.BLH2NED_vec(pts_lla, datum_ecef)
# In this C++ implementation the results (Point.x = East, Point.y = North)
x_coords = [p.y for p in pts_local]
y_coords = [p.x for p in pts_local]

# 绘制轨迹图
plt.figure(figsize=(10, 8))
plt.plot(x_coords, y_coords, marker='o', color='b', markersize=1, linestyle='-',)
plt.title('GPS Trajectory')
plt.xlabel('East')
plt.ylabel('North')
plt.grid(True)
plt.legend()
plt.savefig('gps_trajectory.png', dpi=600)
