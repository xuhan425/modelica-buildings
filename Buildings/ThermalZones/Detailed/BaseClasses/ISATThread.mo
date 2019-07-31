within Buildings.ThermalZones.Detailed.BaseClasses;
class ISATThread "class used to handle ISAT thread"
   extends ExternalObject;
   // constructor
   function constructor "allocate memeory for cosimulation variables"
    output ISATThread ISATThre "the handler of ISAT thread";
    external"C" ISATThre = isatcosim()   annotation (Include="#include <isatcosim.c>",
      IncludeDirectory="modelica://Buildings/Resources/C-Sources",
      LibraryDirectory="modelica://Buildings/Resources/Library", Library="isat");
     annotation (Documentation(info="<html>
<p>
Constructor allocates memory for co-simulation variables when co-simulation starts
</html>",   revisions="<html>
<ul>
<li>
July 21, 2019, by Xu Han and Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));
   end constructor;

   // destructor
   function destructor "release isat.dll or isat.so"
    input ISATThread ISATThre "the handler of ISAT thread";
    external"C" isatSendStopCommand(ISATThre) annotation (Include="#include <isatSendStopCommand.c>",
      IncludeDirectory="modelica://Buildings/Resources/C-Sources",
      LibraryDirectory="modelica://Buildings/Resources/Library", Library="isat");
      annotation (Documentation(info="<html>
<p>
Destructor sends stop command to ISAT and releases memory for co-simulation variables at the end of the simulation

</html>",   revisions="<html>
<ul>
<li>
July 21, 2019, by Xu Han and Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));
   end destructor;
     annotation (Documentation(info="<html>
<p>
Class derived from <code>ExternalObject</code> having two local external function definition,
named <code>destructor</code> and <code>constructor</code> respectively.
</html>",   revisions="<html>
<ul>
<li>
July 21, 2019, by Xu Han and Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

end ISATThread;