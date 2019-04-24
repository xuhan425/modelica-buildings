within Buildings.ThermalZones.Detailed.BaseClasses;
function isatExchangeData "Exchange data between ISAT and Modelica"
  input Integer flag "Communication flag to ISAT";
  input Modelica.SIunits.Time t "Current Modelica simulation time to ISAT";
  input Modelica.SIunits.Time dt(min=100*Modelica.Constants.eps)
    "Requested synchronization time step size";
  input Real[nU] u "Input to ISAT";
  input Integer nU "Number of inputs to ISAT";
  input Integer nY "Number of outputs from ISAT";
  output Modelica.SIunits.Time modTimRea "Current model time from ISAT";
  output Real[nY] y "Output computed by ISAT";
  output Integer retVal "Return value for ISAT simulation status";
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
This function calls a C function to conduct the data exchange between Modelica and ISAT program during the coupled simulation.</p>
</html>", revisions="<html>
<ul>
<li>
April 23, 2019, by Xu Han, Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

end isatExchangeData;
