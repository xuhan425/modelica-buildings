within Buildings.Fluid.Actuators.Valves;
model TwoWayLinear "Two way valve with linear flow characteristics"
  extends BaseClasses.PartialTwoWayValve;

equation
  phi = l + y_actual * (1 - l);
annotation (
defaultComponentName="val",
Documentation(info="<html>
<p>
Two way valve with linear opening characteristic.
</p><p>
This model is based on the partial valve model 
<a href=\"modelica://Buildings.Fluid.Actuators.BaseClasses.PartialTwoWayValve\">
Buildings.Fluid.Actuators.BaseClasses.PartialTwoWayValve</a>. 
Check this model for more information, such
as the leakage flow or regularization near the origin.
</p>
</html>",
revisions="<html>
<ul>
<li>
June 3, 2008 by Michael Wetter:<br>
First implementation.
</li>
</ul>
</html>"));
end TwoWayLinear;
