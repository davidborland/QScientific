# QScientific
A set of Qt widgets for scientific visualization

* QDoubleSlider:  Basically just some glue to combine a QSlider with a QDoubleSpinBox to enable floating-point sliders (Qt only has integer-valued sliders).

* QScientificSpinBox:  A QDoubleSpinBox that can display in scientific notation.

* QDualValue:  A widget that controls two values via horizontal and vertical position of a 2D slider handle.  May be useful for things like window/level control.



![image](https://user-images.githubusercontent.com/289957/222539098-9ba0dc7d-82fe-43c4-ac13-f857a4442234.png)


* QNonlinearSlider:  An abstract base class for sliders that use a nonlinear function to map slider position to data value.

* QPowerSlider:  A QNonlinearSlider that uses a user-controlled power function to map slider position to data value.  The user can interactively control the exponent used for the power function.


![image](https://user-images.githubusercontent.com/289957/222539129-96d210b3-812b-4ef8-8bd4-f720bfc6d78f.png)

* QExploratorySlider:  A QNonlinearSlider that uses a combination of two power functions to enable the user to obtain much better precision in a neighborhood around an interactively-specified data value, or between the data value and the ends of the curve.


![image](https://user-images.githubusercontent.com/289957/222539174-15eeac73-084b-4b9a-a5a1-1c56c81cd3dd.png)
