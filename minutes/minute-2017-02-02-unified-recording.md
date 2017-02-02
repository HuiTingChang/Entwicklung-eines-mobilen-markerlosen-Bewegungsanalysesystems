# Minute of 2 February 2017 (Thu) "Unified Recording"

By Yushin Washio

Attendance: Nuno Alves, Yuliia Danylkina, Yushin Washio

## Activities

### AnyBody

As our software needs to provide data to AnyBody, Yuliia has started researching about the biomechanic data format C3D that is supported by it for import.

### Test run with input devices

Yushin’s QtConcurrent use (as a replacement of ppl.h) seems to have introduced faulty pointer use in run-time when connected with a kinect input. Reverted the change for now.

The color image from kinect is striped on Yushin’s machine, while it’s ok on Nuno’s. No explanation found.

### emma.ini for ApplicationData

Yushin has started using the [inih](/inc/inih) library for handling a configuration file of the INI format. The [emma.ini](/EMMA/EMMA/emma.ini) file is read and used in [ApplicationData.cpp](/EMMA/EMMA/ApplicationData.cpp). It should also be used for initial GUI configuration as discussed on the [12th January](minute-2017-01-12.md).

### GUI

Plotting could be done with [QWT](http://qwt.sourceforge.net/index.html), as some HIA employee has experience with it, or [QCustomPlot](http://www.qcustomplot.com/documentation/), if that turns out to be better/easier.

Yuliia will ask if Inna would take on this task.

The current GUI is a mixture of QtDesigner-produced and hard-coded objects. It should be all transferred to the QtDesigner [emma.ui](/EMMA/EMMA/emma.ui).

### Unified recording

In order to organize all relevant measurement data, we need a class `State` that contains the values for one instant of time. The format of the temporary data storage (csv or binary) would be then derived from it. We defined what methods the `State` class should provide in the issue #2

Nuno will start implementing the `State` class.

Currently the main loop is running in [cvCapture.cpp](../EMMA/EMMA/body/cvCapture.cpp). This should periodically call update and getter functions of the `State` class. The `State` class in turn should call getter functions of body and balance_board. It seems that balance_board does not provide a getter function. Yuliia will look into committing it, as it should’ve been locally implemented.

Yushin will start implementing the continuous storage of the series of measurements (`State` instances).

## Next session

Next Monday the 6th February, Yuliia will be on vacation, so we can’t meet at HIA. Yushin will try to book a room at [E.ON ERC](http://www.eonerc.rwth-aachen.de/cms/E-ON-ERC/Footer/Service/~dmwt/Kontakt-und-Lageplaene/lidx/1/). If that isn’t possible, we’ll probably meet at some RWTH learning room.

## Absence

* Hui-Ting is on vacation (announced, to be compensated by home office)
* Inna is sick
* Gilbert didn’t report
