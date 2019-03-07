within Buildings.ThermalZones.Detailed.BaseClasses;
function isatExchangeData "Exchange data between isat and Modelica"
  input Integer flag "Communication flag to isat";
  input Modelica.SIunits.Time t "Current Modelica simulation time to isat";
  input Modelica.SIunits.Time dt(min=100*Modelica.Constants.eps)
    "Requested synchronization time step size";
  input Real[nU] u "Input to isat";
  input Integer nU "Number of inputs to isat";
  input Integer nY "Number of outputs from isat";
  output Modelica.SIunits.Time modTimRea "Current model time from isat";
  output Real[nY] y "Output computed by isat";
  output Integer retVal "Return value for isat simulation status";
external"C" retVal = isatExchangeData(
    t,
    dt,
    u,
    nU,
    nY,
    modTimRea,
    y) annotation (Include="#include <isatExchangeData.c>", IncludeDirectory=
        "modelica://Buildings/Resources/C-Sources");
  annotation (Documentation(info="<html>
<p>
This function calls a C function to conduct the data exchange between Modelica and isat program during the coupled simulation.</p>
</html>", revisions="<html>
<ul>
<li>
August 16, 2013, by Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

end isatExchangeData;
