# EMcsv

**EMcsv** is the export format supported by EMMA. It is a csv file produced by the RFC-4180-compliant [text-csv](https://github.com/roman-kashitsyn/text-csv) library. Each entry (line) represents a state with the latest input data available at that time. There is no header in the file and all lines contain the following data (in this order):

* timestamp as milliseconds since Epoch (unix, as implemented by Qt5 QDateTime) in UTC
* 25 joint position entries (3 values each, representing x, y, z coordinates in the Kinect Coordinates)
* 25 bone relative angles (3 values each, representing α, β, γ as described in [Doc_relative_coordination_for_angle_calc_and_angleSize](/Doku/Doc_relative_coordination_for_angle_calc_and_angleSize))

## Joints

Joints are based on the ones defined in the Kinect SDK. In EMMA they are numbered (relevant for the CSV):

```
 0 SpineBase
 1 SpineMid
 2 Neck
 3 Head	
 4 ShoulderLeft
 5 ElbowLeft
 6 WristLeft
 7 HandLeft
 8 ShoulderRight
 9 ElbowRight
10 WristRight
11 HandRight
12 HipLeft
13 KneeLeft
14 AnkleLeft
15 FootLeft
16 HipRight
17 KneeRight
18 AnkleRight
19 FootRight
20 SpineShoulder
21 HandTipLeft
22 ThumbLeft
23 HandTipRight
24 ThumbRight
```

## Bones

Bones are defined as the link between two joints, identified by the same number as its child joint. See more about joint hierarchy ("child" and "parent") in (Doc_relative_coordination_for_angle_calc_and_angleSize)[Doku/Doc_relative_coordination_for_angle_calc_and_angleSize)

## Kinect Coordinates

EMMA uses the BodySpace from the Kinect SDK. See [the Kinect SDK documentation](TODO)
