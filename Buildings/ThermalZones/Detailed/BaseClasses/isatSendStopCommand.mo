within Buildings.ThermalZones.Detailed.BaseClasses;
function isatSendStopCommand "Send the stop command to isat"

external"C" isatSendStopCommand() annotation (Include=
        "#include <isatSendStopCommand.c>", IncludeDirectory=
        "modelica://Buildings/Resources/C-Sources");

  annotation (Documentation(info="<html>
<p>
This function calls a C function to send a stop command to isat to stop the isat simulation.</html>",
        revisions="<html>
<ul>
<li>
August 16, 2013, by Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

end isatSendStopCommand;
