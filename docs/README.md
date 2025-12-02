Coordinate conversion notes
===========================

- `Point` parameters: x -> latitude (degrees), y -> longitude (degrees), z -> height (meters).
- `BLH2XYZ(Point)` converts LLA (lat, lon, height) to ECEF (X, Y, Z) in meters.
- `BLH2NED(Point p, Point dp)` converts LLA point `p` to a local NED coordinate system relative to a datum `dp` which must be provided in ECEF (use `BLH2XYZ` on the datum LLA first). When using the `BLH2NED_vec` convenience method, pass the datum as ECEF.
- `BLH2NEDK(Point p, Point dp)` is a simpler approximation; `dp` is expected in LLA; it directly returns East/North using local scale factors Kx/Ky; good for small-area approximations.

Notes on usage in Python:

- Use `BLH2NEDK` when you prefer to pass the datum as a LLA point (easier). This function is an approximation and more suitable for small-area conversions.
- For better accuracy across larger areas, use `BLH2NED` with datum in ECEF.
- The C++ code returns `Point.x` as East, and `Point.y` as North (the code uses this convention). 

Example: Convert LLA list to NED relative to first point (approximation)
```python
cc = CoordinateConvert()
pts_lla = [Point(lat, lon, 0) for lat, lon in zip(latitudes, longitudes)]
pts_ned = cc.BLH2NED_vec(pts_lla, pts_lla[0])  # using BLH2NEDK / BLH2NED as needed
```
