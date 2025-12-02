"""Run with uv tool if available:
	uv venv .venv -p 3.13 --seed
	uv sync
	uv add pybind11 --dev
	uv run python setup.py build_ext --inplace
	uv run ./main.py
"""
import pandas as pd
import matplotlib.pyplot as plt
try:
	import coordinate_convert as cc_mod
	from coordinate_convert import Point, CoordinateConvert
	use_cpp = True
except Exception:
	from pyproj import Proj, transform
	use_cpp = False

# 加载GPS数据
file_path = '20030801.xls'  # 请确保使用正确的文件路径
data = pd.read_excel(file_path)

# 提取经纬度数据
latitudes = data.iloc[:,4]  # 纬度
longitudes = data.iloc[:,5]  # 经度

if use_cpp:
	# Use the C++ converter via pybind11
	cc = CoordinateConvert() # pyright: ignore[reportPossiblyUnboundVariable]
	pts_lla = [Point(float(lat), float(lon), 0.0) for lat, lon in zip(latitudes, longitudes)] # pyright: ignore[reportPossiblyUnboundVariable]
	# pick a datum — first point
	datum_lla = pts_lla[0]
	# For accurate conversion, convert the datum to ECEF and use BLH2NED (BLH2NED_vec) which expects an ECEF datum
	datum_ecef = cc.BLH2XYZ(datum_lla)
	pts_local = cc.BLH2NED_vec(pts_lla, datum_ecef)
	# In this C++ implementation the results (Point.x = East, Point.y = North)
	x_coords = [p.x for p in pts_local]
	y_coords = [p.y for p in pts_local]
else:
	# 定义地理坐标系和投影坐标系
	# WGS84坐标系：用于经纬度，EPSG:4326
	wgs84 = Proj(init='epsg:4326')
	# UTM投影坐标系，通常选择一个适合你区域的投影，如EPSG:32633 (UTM Zone 33N)
	utm = Proj(init='epsg:32633')
	# 将经纬度转换为UTM坐标（x, y）
	x_coords, y_coords = transform(wgs84, utm, longitudes.values, latitudes.values)

# 绘制轨迹图
plt.figure(figsize=(10, 6))
plt.plot(x_coords, y_coords, marker='o', color='b', markersize=2, linestyle='-', label="GPS Trajectory")
plt.title('GPS Trajectory in UTM Coordinates')
plt.xlabel('UTM Easting (meters)')
plt.ylabel('UTM Northing (meters)')
plt.grid(True)
plt.legend()
plt.savefig('gps_trajectory_utm.png', dpi=300)
