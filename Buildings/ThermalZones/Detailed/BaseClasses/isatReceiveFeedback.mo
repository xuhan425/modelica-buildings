within Buildings.ThermalZones.Detailed.BaseClasses;
function isatReceiveFeedback "Receive the feedback from isat"
  output Integer retVal
    "Return value of the function (0 indicates isat successfully stopped.)";
external"C" retVal = isatReceiveFeedback() annotation (Include=
        "#include <isatReceiveFeedback.c>", IncludeDirectory=
        "modelica://Buildings/Resources/C-Sources");
  annotation (Documentation(info="<html>
<p>
This function calls a C function to receive the feedback from isat about the status of performing a Stop command sent by Modelica.</html>",
        revisions="<html>
<ul>
<li>
August 16, 2013, by Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

end isatReceiveFeedback;
