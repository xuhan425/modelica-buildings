within Buildings.ThermalZones.Detailed.Examples;
package ISAT "Package that tests the models for coupled simulation between Modelica and ISAT"
  extends Modelica.Icons.ExamplesPackage;


annotation (Documentation(info="<html>
<p>
This package tests the coupled simulation of the model
<a href=\"Buildings.ThermalZones.Detailed.CFD\">Buildings.ThermalZones.Detailed.CFD</a> with the Fast Fluid Dynamics (FFD) program.
Different cases with various boundary conditions are evaluated.
The models in this package do not represent realistic buildings, but
are rather designed to test the coupled simulation.
</p>
</html>", revisions="<html>
<ul>
<li>
April 5, 2020, by Xu Han, Wangda Zuo and Michael Wetter:<br/>
First implementation.
</li>
</ul>
</html>"));
end ISAT;