## float CurrentState::angleSizeCalc(EMMA::Joints jointNumber)


## JointRelativeAngles anglesInRelativeCoordinateSystem()

This function calculates the angles between parent->child and x-/y-/z-axis.
The Hierarchy of joints:
![](JointHierarchy.jpg)
The coordination system of "Relative to parent joint" is defined in the [website of iPi Soft](http://docs.ipisoft.com/iPi_Biomech_Add-on)
- Y-Axis is vertical to the floor(XZ plane).
- X-Axis is directed along left arm in [T-pose]. (http://docs.ipisoft.com/File:iPi-Biomech-1-2.png)
- Z-Axis is the cross-product of X-Axis and Y-Axis.
- Center/Origine coincides with the coordinates of the parent joint.

It returns JointRelativeAngles, which is a QMap<uint, Angles3D>. 
Anggles3D has 3 values: x, y and z, which coresponds to the angle between parent->child and x-axis, the angle between parent->child and y-axis and the angle between parent->child and z-axis.
In this QMap, you can access 

