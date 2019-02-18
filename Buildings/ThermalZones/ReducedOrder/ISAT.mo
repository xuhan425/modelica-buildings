within Buildings.ThermalZones.ReducedOrder;
package ISAT "Package to call ISAT dll"
  extends Modelica.Icons.VariantsPackage;

  package UsersGuide "User's Guide"
    extends Modelica.Icons.Information;

  annotation (preferredView="info",
  Documentation(info="<html>
<p>
This package contains classes that call Python functions.
The classes can be used to send data to Python functions,
and to obtain data from Python functions.
This allows for example to use Python to communicate
with web services, with hardware, or to do other computations
inside a Python module.
</p>
<p>
The code has been tested with Python 2.7 on
Linux 32 and 64 bit and Windows 32 and 64 bit.
</p>
<h4>Software configuration to use classes from this package</h4>
<p>
To use classes from this package, a Python 2.7
runtime environment must be installed.
Also, the system environment variable
<code>PYTHONPATH</code> must be set in order for Python
to find the modules that contain the functions.
These modules are stored in the directory
<code>Buildings/Resources/Python-Sources</code>.
In addition, an environment variable (<code>LD_LIBRARY_PATH</code> on Linux
and <code>PATH</code> on Windows) must be set in order for a simulation
environment to find the dynamically linked libraries.
The table below explains how to set these variables for
various system configurations.
</p>
<p>
Because some Python libraries may also link to compiled C code, we
recommend that if you are using a 64-bit operating system, you configure
Dymola to generate 64 bit code. Configuring the compilation
can be done by entering on the
Dymola command line the assignment
<code>Advanced.CompileWith64=1</code> for 32-bit, or
<code>Advanced.CompileWith64=2</code> for 64-bit.
</p>
  <table summary=\"summary\" border=\"1\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse:collapse;\">
  <tr>
      <th>System</th>
      <th>Settings</th>
   </tr>
  <!-- =================================================================== -->
    <tr>
      <td>Linux 32 bit, Dymola 2016</td>
      <td>
      <p>
      Enter on a console the commands
      </p>
<pre>
  export PYTHONPATH=${PYTHONPATH}:\"Path_To_Buildings_Library\"/Resources/Python-Sources
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:\"Path_To_Buildings_Library\"/Resources/Library/linux32
  </pre>
  <p>
  Alternatively, these lines could be added to the file <code>~/.bashrc</code>.
  </p>
      </td>
    </tr>
    <tr>
  <!-- =================================================================== -->
      <td>Linux 64 bit, Dymola 2016</td>
      <td>
      <p>
      Use the same commands as for <i>Linux 64 bit, Dymola 2016</i>
      because Dymola 2016 generates by default 32 bit code.
      </p>
      <p>
      However, if you load other Python libraries such as numpy, you need to make sure
      that Dymola compiles 64 bit and uses the 64 bit library from
      <code>Buildings/Resources/Library/linux64</code>.
      To do so, enter on a console the commands
      </p>
<pre>
  export PYTHONPATH=${PYTHONPATH}:\"Path_To_Buildings_Library\"/Resources/Python-Sources
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:\"Path_To_Buildings_Library\"/Resources/Library/linux64
  </pre>
  <p>
Alternatively, these lines could be added to the file <code>~/.bashrc</code>.</p>
<p>
Next, in the Dymola command window, set
</p>
<pre>
 Advanced.CompileWith64=2;
</pre>
      </td>
    </tr>
  <!-- =================================================================== -->
    <tr>
      <td>Linux 32 bit, Dymola 2014</td>
      <td>
Enter on a console the commands
<pre>
  export PYTHONPATH=${PYTHONPATH}:\"Path_To_Buildings_Library\"/Resources/Python-Sources
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:\"Path_To_Buildings_Library\"/Resources/Library/linux32
</pre>
Alternatively, these lines could be added to the file <code>~/.bashrc</code>.
      </td>
    </tr>
    <tr>
  <!-- =================================================================== -->
      <td>Linux 64 bit, Dymola 2014</td>
      <td>
Use the same commands as for <i>Linux 64 bit, Dymola 2014</i> because Dymola 2014 only generates 32 bit code.
      </td>
    </tr>
  <!-- =================================================================== -->
    <tr>
      <td>Linux 32 bit, Dymola 2013 FD01</td>
      <td>
Enter on a console the commands
<pre>
  export PYTHONPATH=${PYTHONPATH}:\"Path_To_Buildings_Library\"/Resources/Python-Sources
</pre>
Alternatively, these lines could be added to the file <code>~/.bashrc</code>.<br/>
<br/>
Next, modify <code>/opt/dymola/bin/dymola.sh</code> by replacing the line
<pre>
  export LD_LIBRARY_PATH=$DYMOLA/bin/lib
</pre>
with
<pre>
  export LD_LIBRARY_PATH=$DYMOLA/bin/lib:\"Path_To_Buildings_Library\"/Resources/Library/linux32
  export LD_LIBRARY_PATH=$DYMOLA/bin/lib:Resources/Library/linux32
</pre>
      </td>
    </tr>
  <!-- =================================================================== -->
    <tr>
      <td>Linux 64 bit, Dymola 2013 FD01</td>
      <td>
Use the same commands as for <i>Linux 32 bit, Dymola 2013 FD01</i> because Dymola 2013 FD01 only generates 32 bit code.
      </td>
    </tr>
  <!-- =================================================================== -->
    <tr>
      <td>Windows 32 bit, Dymola 2014
      <br/>Windows 64 bit, Dymola 2014
      <br/>Windows 32 bit, Dymola 2013 FD01
      <br/>Windows 64 bit, Dymola 2013 FD01 </td>
      <td>
        Add to the system environment variable <code>PYTHONPATH</code> the directory
        <code>\"Path_To_Buildings_Library\"\\Resources\\Python-Sources</code>.
      </td>
    </tr>
  <!-- =================================================================== -->
  </table>

<h4>Number of values to read to Python and write from Python</h4>
<p>
The parameters <code>nDblWri</code> (or <code>nIntWri</code> or <code>nStrWri</code>)
and <code>nDblRea</code> (or <code>nIntRea</code>) declare
how many double (integer or string) values should be written to, or read from, the Python function.
These values can be zero, in which case the Python function receives no
arguments for this data type, or it must return a list with zero elements.
However, because Modelica does not allow arrays with
zero elements, the arrays
<code>dblWri</code> and <code>dblRea</code>, respectively, must contain exactly one element
if <code>nDblWri=0</code> or <code>nDblRea=0</code>.
In this situation, <code>dblWri</code> is a dummy argument that will not be written to Python,
and <code>dblRea</code> contains a number that must not be used in any model.
</p>

<h4>Arguments of the Python function</h4>
<p>
The arguments of the python functions are, in this order,
floats, integers and strings.
If there is only one element of each data type, then a single value is passed.
If there are multiple elements of each data type, then they are stored in a list.
If there is no value of a data type (such as if <code>nDblWri=0</code>), then the argument is not present.
Thus, if a data type is not present, then the function will <i>not</i> receive an empty list of this data type.
If there are no arguments at all, then the function takes no arguments.</p>
<p>
The table below shows the list of arguments for various combinations where no,
one or two double values, integers and strings are passed as an argument to a Python function.
</p>
 <table summary=\"summary\" border=\"1\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse:collapse;\">
  <tr> <th>nDblWri</th>   <th>nIntWri</th>  <th>nStrWri</th>  <th>Arguments</th>  </tr>
  <tr> <td>1      </td>   <td>0      </td>  <td>0      </td>  <td>1.                            </td></tr>
  <tr> <td>0      </td>   <td>1      </td>  <td>1      </td>  <td>1, \"a\"                        </td></tr>
  <tr> <td>2      </td>   <td>0      </td>  <td>2      </td>  <td>[1.0, 2.0], [\"a\", \"b\"]        </td></tr>
  <tr> <td>1      </td>   <td>1      </td>  <td>1      </td>  <td> 1.0, 2, \"a\"                  </td></tr>
  <tr> <td>1      </td>   <td>2      </td>  <td>0      </td>  <td> 1.0       , [1, 2]           </td></tr>
  <tr> <td>2      </td>   <td>1      </td>  <td>0      </td>  <td>[1.0, 2.0], 1                 </td></tr>
  <tr> <td>2      </td>   <td>2      </td>  <td>2      </td>  <td>[1.0, 2.0], [1, 2], [\"a\", \"b\"]</td></tr>
</table>
<br/>

<h4>Returns values of the Python function</h4>
<p>
The Python function must return their values in the following order:</p>
<ol>
<li>
If the function returns one or multiple double values, then the first return
value must be a double (if <code>nDblRea=1</code>) or a list of doubles
(if <code>nDblRea &gt; 1</code>).
</li>
<li>
If the function returns one or multiple integer values, then the next return
value must be an integer (if <code>nIntRea=1</code>) or a list of integers
(if <code>nIntRea &gt; 1</code>).
</li>
<li>
If <code>nDblRea = nIntRea = 0</code>, then the return values of the function, if any, are
ignored.
</li>
</ol>
<p>
The table below shows valid return types for various combinations where no, one or two double values
and integer values are returned.</p>
 <table summary=\"summary\" border=\"1\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse:collapse;\">
  <tr> <th>nDblRea</th>   <th>nIntRea</th>  <th>Return value</th>  </tr>
  <tr> <td>1      </td>   <td>0      </td>  <td>1.                 </td></tr>
  <tr> <td>0      </td>   <td>1      </td>  <td>1                  </td></tr>
  <tr> <td>2      </td>   <td>0      </td>  <td>[1.0, 2.0]         </td></tr>
  <tr> <td>1      </td>   <td>1      </td>  <td> 1.0, 2            </td></tr>
  <tr> <td>1      </td>   <td>2      </td>  <td> 1.0      , [1, 2] </td></tr>
  <tr> <td>2      </td>   <td>1      </td>  <td>[1.0, 2.0],  1     </td></tr>
  <tr> <td>2      </td>   <td>2      </td>  <td>[1.0, 2.0], [1, 2] </td></tr>
</table>

<!-- Not yet implemented as pure functions are not supported in Dymola 2013 FD01 -->
<h4>Pure Modelica functions (functions without side effects)</h4>
<p>
The functions that exchange data with Python are implemented as <i>pure</i>
Modelica functions.
Pure functions always return the same value if called repeatedly.
If these functions are used to call hardware sensors or web services,
they need to be called from a <code>when</code>-equation.</p>
<p>
See the Modelica language specification for an explanation
of pure and impure functions.
</p>

<h4>Examples</h4>
<p>
The example
<a href=\"modelica://Buildings.Utilities.IO.Python27.Functions.Examples.Exchange\">
Buildings.Utilities.IO.Python27.Functions.Examples.Exchange</a>
contains various calls to different Python functions.
The Python functions are stored in the directory
<code>Buildings/Resources/Python-Sources</code>.
</p>
<p>
The example
<a href=\"modelica://Buildings.Utilities.IO.Python27.Examples.KalmanFilter\">
Buildings.Utilities.IO.Python27.Examples.KalmanFilter</a>
shows how to implement in a Modelica block a call to a Python function.
</p>

<h4>Implementation notes</h4>
<p>
String values cannot be returned from a Python function.
The reason is that Dymola 2013 FD01 generates a compile time error
if a Modelica function returns <code>(Real[nR], Integer[nI], String)</code>.
This will be fixed in Dymola 2014.
(Support request #14983.)
</p>
<h4>Known Issues</h4>
<p>
The Python installation of Ubuntu 14.04 has a bug that causes Dymola
to produce output of the following form:
</p>
<pre>
Traceback (most recent call last):
  File \"/usr/lib/python2.7/site.py\", line 563, in &lt;module&gt;
    main()
  File \"/usr/lib/python2.7/site.py\", line 545, in main
    known_paths = addusersitepackages(known_paths)
  File \"/usr/lib/python2.7/site.py\", line 272, in addusersitepackages
    user_site = getusersitepackages()
  File \"/usr/lib/python2.7/site.py\", line 247, in getusersitepackages
    user_base = getuserbase() # this will also set USER_BASE
  File \"/usr/lib/python2.7/site.py\", line 237, in getuserbase
    USER_BASE = get_config_var('userbase')
  File \"/usr/lib/python2.7/sysconfig.py\", line 578, in get_config_var
    return get_config_vars().get(name)
  File \"/usr/lib/python2.7/sysconfig.py\", line 524, in get_config_vars
    _init_posix(_CONFIG_VARS)
  File \"/usr/lib/python2.7/sysconfig.py\", line 408, in _init_posix
    from _sysconfigdata import build_time_vars
  File \"/usr/lib/python2.7/_sysconfigdata.py\", line 6, in &lt;module&gt;
    from _sysconfigdata_nd import *
ImportError: No module named _sysconfigdat
...(message truncated)
</pre>
<p>
As a work-around, type in a shell the commands
</p>
<pre>
$ cd /usr/lib/python2.7
$ sudo ln -s plat-x86_64-linux-gnu/_sysconfigdata_nd.py .
</pre>
<p>
See also <a href=\"https://bugs.launchpad.net/ubuntu/+source/python2.7/+bug/1115466\">
https://bugs.launchpad.net/ubuntu/+source/python2.7/+bug/1115466</a>.
</p>
</html>"));
  end UsersGuide;

  model ISAT "Block that exchanges a vector of real values with ISAT dll"
    extends Modelica.Blocks.Interfaces.DiscreteBlock(
      startTime=0,
      firstTrigger(fixed=true, start=false));

    parameter String moduleName
      "Name of the python module that contains the function";
    parameter String functionName=moduleName "Name of the python function";

    parameter Integer nDblWri(min=1) "Number of double values to write to Python";
    parameter Integer nDblRea(min=1)
      "Number of double values to be read from the Python";
    parameter Integer flaDblWri[nDblWri] = zeros(nDblWri)
      "Flag for double values (0: use current value, 1: use average over interval, 2: use integral over interval)";

    Modelica.Blocks.Interfaces.RealInput uR[nDblWri]
      "Real inputs to be sent to Python"
      annotation (Placement(transformation(extent={{-140,-20},{-100,20}})));
    Modelica.Blocks.Interfaces.RealOutput yR[nDblRea]
      "Real outputs received from Python"
      annotation (Placement(transformation(extent={{100,-10},{120,10}})));

    Real uRInt[nDblWri] "Value of integral";
    Real uRIntPre[nDblWri] "Value of integral at previous sampling instance";
    Real uRWri[nDblWri] "Value to be sent to Python";
  initial equation
     uRWri    =  pre(uR);
     uRInt    =  zeros(nDblWri);
     uRIntPre =  zeros(nDblWri);
     for i in 1:nDblWri loop
       assert(flaDblWri[i]>=0 and flaDblWri[i]<=2,
          "Parameter flaDblWri out of range for " + String(i) + "-th component.");
     end for;
     // The assignment of yR avoids the warning
     // "initial conditions for variables of type Real are not fully specified".
     // At startTime, the sampleTrigger is true and hence this value will
     // be overwritten.
     yR = zeros(nDblRea);
  equation
     for i in 1:nDblWri loop
        der(uRInt[i]) = if (flaDblWri[i] > 0) then uR[i] else 0;
     end for;

    when {sampleTrigger} then
       // Compute value that will be sent to Python
       for i in 1:nDblWri loop
         if (flaDblWri[i] == 0) then
           uRWri[i] = pre(uR[i]);                 // Send the current value.
         else
           uRWri[i] = uRInt[i] - pre(uRIntPre[i]);     // Integral over the sampling interval
           if (flaDblWri[i] == 1) then
              uRWri[i] =  uRWri[i]/samplePeriod;  // Average value over the sampling interval
           end if;
         end if;
        end for;

      // Exchange data
      yR =Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
          moduleName=moduleName,
          functionName=functionName,
          dblWri=uRWri,
          intWri={0},
          nDblWri=nDblWri,
          nDblRea=nDblRea,
          nIntWri=0,
          nIntRea=0,
          nStrWri=0,
          strWri={""});

      // Store current value of integral
    uRIntPre= uRInt;
    end when;

    annotation (defaultComponentName="pyt",
   Icon(coordinateSystem(
            preserveAspectRatio=false, extent={{-100,-100},{100,100}})),
      Documentation(info="<html>
Block that exchanges data with a Python function.<br/>
<p>
For each element in the input vector <code>uR[nDblWri]</code>,
the value of the flag <code>flaDblWri[nDblWri]</code> determines whether
the current value, the average over the sampling interval or the integral
over the sampling interval is sent to Python. The following three options are allowed:
</p>
<br/>
<table summary=\"summary\" border=\"1\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse:collapse;\">
<tr>
<td>
flaDblWri[i]
</td>
<td>
Value sent to Python
</td>
</tr>
<tr>
<td>
0
</td>
<td>
Current value of uR[i]
</td>
</tr>
<tr>
<td>
1
</td>
<td>
Average value of uR[i] over the sampling interval
</td>
</tr>
<tr>
<td>
2
</td>
<td>
Integral of uR[i] over the sampling interval
</td>
</tr>
</table>
<br/>
</html>",   revisions="<html>
<ul>
<li>
June 9, 2015 by Michael Wetter:<br/>
Set <code>firstTrigger(fixed=true, start=false)</code> to avoid a
warning about unspecified initial conditions if
<a href=\"modelica://Buildings.Utilities.IO.Python27.Examples.KalmanFilter\">
Buildings.Utilities.IO.Python27.Examples.KalmanFilter</a>
is translated
using pedantic mode in Dymola 2016.
This is for
<a href=\"https://github.com/lbl-srg/modelica-buildings/issues/426\">#426</a>.
</li>
<li>
September 29, 2014, by Michael Wetter:<br/>
Changed <code>algorithm</code> to <code>equation</code> section
and assigned start values to avoid a translation warning in
Dymola.
</li>
<li>
February 5, 2013, by Michael Wetter:<br/>
First implementation,
based on
<a href=\"modelica://Buildings.Utilities.IO.BCVTB.BCVTB\">
Buildings.Utilities.IO.BCVTB.BCVTB</a>.
</li>
</ul>
</html>"));
  end ISAT;

  package Functions "Package with functions that call Python"
    extends Modelica.Icons.BasesPackage;

    function ISATExchange "Function that communicates with ISAT"
      input String moduleName
        "Name of the python module that contains the function";
      input String functionName=moduleName "Name of the python function";

      input Real    dblWri[max(1, nDblWri)] "Double values to write";
      input Integer intWri[max(1, nIntWri)] "Integer values to write";
      input String  strWri[max(1, nStrWri)] "String values to write";

      input Integer nDblWri(min=0) "Number of double values to write";
      input Integer nDblRea(min=0) "Number of double values to read";

      input Integer nIntWri(min=0) "Number of integer values to write";
      input Integer nIntRea(min=0) "Number of integer values to read";

      input Integer nStrWri(min=0) "Number of strings to write";
    //  input Integer nStrRea(min=0) "Number of strings to read";
    //  input Integer strLenRea(min=0)
    //    "Maximum length of each string that is read. If exceeded, the simulation stops with an error";

      output Real    dblRea[max(1, nDblRea)] "Double values returned by Python";
      output Integer intRea[max(1, nIntRea)] "Integer values returned by Python";
    protected
      String pytPat "Value of PYTHONPATH environment variable";
      String pytPatBuildings "PYTHONPATH of Buildings library";
      Boolean havePytPat "true if PYTHONPATH is already set by the user";
    //--  String filNam = "file://Utilities/IO/Python27/UsersGuide/package.mo"
    //--    "Name to a file of the Buildings library";
    algorithm
     // Get the directory to Buildings/Resources/Python-Sources
    //-- The lines below do not work in Dymola 2014 due to an issue with the loadResource
    //-- (ticket #15168). This will be fixed in future versions.
    //-- pytPatBuildings := Buildings.BoundaryConditions.WeatherData.BaseClasses.getAbsolutePath(uri=filNam);
    //-- pytPatBuildings := Modelica.Utilities.Strings.replace(
    //--   string=pytPatBuildings,
    //--   searchString=filNam,
    //--   replaceString="Resources/Python-Sources");
     // The next line is a temporary fix for the above problem
     pytPatBuildings := "Resources/Python-Sources";
     // Update the PYTHONPATH variable
     (pytPat, havePytPat) :=Modelica.Utilities.System.getEnvironmentVariable("PYTHONPATH");
     if havePytPat then
       Modelica.Utilities.System.setEnvironmentVariable(name="PYTHONPATH",
          content=pytPat + ":" + pytPatBuildings);
     else
       Modelica.Utilities.System.setEnvironmentVariable(name="PYTHONPATH",
          content=pytPatBuildings);
     end if;
     // Call the exchange function
     (dblRea, intRea) :=BaseClasses.isatExchangeData(
            moduleName=moduleName,
            functionName=functionName,
            dblWri=dblWri,
            intWri=intWri,
            strWri=strWri,
            nDblWri=nDblWri,
            nDblRea=nDblRea,
            nIntWri=nIntWri,
            nIntRea=nIntRea,
            nStrWri=nStrWri);

     // Change the PYTHONPATH back to what it was so that the function has no
     // side effects.
     if havePytPat then
       Modelica.Utilities.System.setEnvironmentVariable(name="PYTHONPATH",
          content=pytPat);
     else
       Modelica.Utilities.System.setEnvironmentVariable(name="PYTHONPATH",
          content="");
     end if;

      annotation (Documentation(info="<html>
<p>
This function is a wrapper for
<a href=\"modelica://Buildings.Utilities.IO.Python27.Functions.BaseClasses.exchange\">
Buildings.Utilities.IO.Python27.Functions.BaseClasses.exchange</a>.
It adds the directory <code>modelica://Buildings/Resources/Python-Sources</code>
to the environment variable <code>PYTHONPATH</code>
prior to calling the function that exchanges data with Python.
After the function call, the <code>PYTHONPATH</code> is set back to what
it used to be when entering this function.
See
<a href=\"modelica://Buildings.Utilities.IO.Python27.UsersGuide\">
Buildings.Utilities.IO.Python27.UsersGuide</a>
for instructions, and
<a href=\"modelica://Buildings.Utilities.IO.Python27.Functions.Examples\">
Buildings.Utilities.IO.Python27.Functions.Examples</a>
for examples.
</p>
</html>",     revisions="<html>
<ul>
<li>
May 2, 2013, by Michael Wetter:<br/>
First implementation.
</li>
</ul>
</html>"));
    end ISATExchange;

    package Examples "Collection of models that illustrate model use and test models"
      extends Modelica.Icons.ExamplesPackage;

      model Exchange "Test model for exchange function"
        extends Modelica.Icons.Example;

        Real    yR1[1] "Real function value";
        Integer yI1[1] "Integer function value";
        Real    yR2[2] "Real function value";
        Integer yI2[2] "Integer function value";
      algorithm
        yR1 :=Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="r1_r1",
                dblWri={2.0},
                intWri={0},
                nDblWri=1,
                nDblRea=1,
                nIntWri=0,
                nIntRea=0,
                nStrWri=0,
                strWri={""});
          assert(abs(4-yR1[1]) < 1e-5, "Error in function r1_r1");

        yR1 :=Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="r2_r1",
                dblWri={2.0,3.0},
                intWri={0},
                nDblWri=2,
                nDblRea=1,
                nIntWri=0,
                nIntRea=0,
                nStrWri=0,
                strWri={""});
          assert(abs(6-yR1[1]) < 1e-5, "Error in function r2_r1");

        yR2 :=Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="r1_r2",
                dblWri={2.0},
                intWri={0},
                nDblWri=1,
                nDblRea=2,
                nIntWri=0,
                nIntRea=0,
                nStrWri=0,
                strWri={""});
        assert(abs(yR2[1]-2) + abs(yR2[2]-4) < 1E-5, "Error in function r1_r2");

        // In the call below, yR1 is a dummy variable, as exchange returns (Real[1], Integer[1])
        (yR1,yI1) :=
          Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="i1_i1",
                dblWri={0.0},
                intWri={3},
                nDblWri=0,
                nDblRea=0,
                nIntWri=1,
                nIntRea=1,
                nStrWri=0,
                strWri={""});
        assert((6-yI1[1]) < 1e-5, "Error in function i1_i1");

        // In the call below, yR1 is a dummy variable, as exchange returns (Real[1], Integer[2])
        (yR1,yI2) :=
          Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="i1_i2",
                dblWri={0.0},
                intWri={2},
                nDblWri=0,
                nDblRea=0,
                nIntWri=1,
                nIntRea=2,
                nStrWri=0,
                strWri={""});
        assert(abs(yI2[1]-2) + abs(yI2[2]-4) < 1E-5, "Error in function i1_i2");

        yR2 :=Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="r1i1_r2",
                dblWri={0.3},
                intWri={2},
                nDblWri=1,
                nDblRea=2,
                nIntWri=1,
                nIntRea=0,
                nStrWri=0,
                strWri={""});
        assert(abs(yR2[1]-0.6) + abs(yI2[2]-4) < 1E-5, "Error in function r1i1_r2");

        // From Modelica, write a number to a text file, and from Python, read the text file
        // and return the number.
        Modelica.Utilities.Files.removeFile(fileName="tmp-TestPythonInterface.txt");
        Modelica.Utilities.Streams.print(string="1.23", fileName="tmp-TestPythonInterface.txt");
        yR1 :=Buildings.ThermalZones.ReducedOrder.ISAT.Functions.ISATExchange(
                moduleName="testFunctions",
                functionName="s2_r1",
                dblWri={0.0},
                intWri={0},
                nDblWri=0,
                nDblRea=1,
                nIntWri=0,
                nIntRea=0,
                nStrWri=2,
                strWri={"tmp-TestPythonInterface","txt"});
         assert(abs(yR1[1]-1.23) < 1E-5, "Error in function s2_r1");

        annotation (
      experiment(Tolerance=1e-6, StopTime=1.0),
      __Dymola_Commands(file="modelica://Buildings/Resources/Scripts/Dymola/Utilities/IO/Python27/Functions/Examples/Exchange.mos"
              "Simulate and plot"),
      Documentation(info="<html>
<p>
This example calls various functions in the Python module <code>testFunctions.py</code>.
It tests whether arguments and return values are passed correctly.
The functions in  <code>testFunctions.py</code> are very simple in order to test
whether they compute correctly, and whether the data conversion between Modelica and
Python is implemented correctly.
Each call to Python is followed by an <code>assert</code> statement which terminates
the simulation if the return value is different from the expected value.
</p>
</html>",       revisions="<html>
<ul>
<li>
February 2, 2016, by Michael Wetter:<br/>
Changed constant arguments of exchange function from <code>int</code> to <code>double</code>.
This is required for OpenModelica.
</li>
<li>
January 31, 2013, by Michael Wetter:<br/>
First implementation.
</li>
</ul>
</html>"));
      end Exchange;
    annotation (preferredView="info", Documentation(info="<html>
<p>
This package contains examples for the use of models that can be found in
<a href=\"modelica://Buildings.Utilities.IO.Python27.Functions\">
Buildings.Utilities.IO.Python27.Functions</a>.
</p>
<p>
The examples demonstrate how to call Python functions from Modelica.
</p>
</html>"));
    end Examples;

    package BaseClasses "Package with functions that call Python"
      extends Modelica.Icons.BasesPackage;

      function isatExchangeData "Function that communicates with ISAT"
        input String moduleName
          "Name of the python module that contains the function";
        input String functionName=moduleName "Name of the python function";

        input Real    dblWri[max(1, nDblWri)] "Double values to write";
        input Integer intWri[max(1, nIntWri)] "Integer values to write";
        input String  strWri[max(1, nStrWri)] "String values to write";

        input Integer nDblWri(min=0) "Number of double values to write";
        input Integer nDblRea(min=0) "Number of double values to read";

        input Integer nIntWri(min=0) "Number of integer values to write";
        input Integer nIntRea(min=0) "Number of integer values to read";

        input Integer nStrWri(min=0) "Number of strings to write";
      //  input Integer nStrRea(min=0) "Number of strings to read";
      //  input Integer strLenRea(min=0)
      //    "Maximum length of each string that is read. If exceeded, the simulation stops with an error";

        output Real    dblRea[max(1, nDblRea)] "Double values returned by Python";
        output Integer intRea[max(1, nIntRea)] "Integer values returned by Python";

        external "C" pythonExchangeValues(moduleName, functionName,
                                          dblWri, nDblWri,
                                          dblRea, nDblRea,
                                          intWri, nIntWri,
                                          intRea, nIntRea,
                                          strWri, nStrWri)
          annotation (Library={"ModelicaBuildingsPython2.7",  "python2.7"},
            LibraryDirectory={"modelica://Buildings/Resources/Library"},
            IncludeDirectory="modelica://Buildings/Resources/C-Sources",
            Include="#include \"python27Wrapper.c\"",
            __iti_dll = "ITI_ModelicaBuildingsPython2.7.dll",
            __iti_dllNoExport = true);

        annotation (Documentation(info="<html>
<p>
This function exchanges data with Python.
See
<a href=\"modelica://Buildings.Utilities.IO.Python27.UsersGuide\">
Buildings.Utilities.IO.Python27.UsersGuide</a>
for instructions, and
<a href=\"modelica://Buildings.Utilities.IO.Python27.Functions.Examples\">
Buildings.Utilities.IO.Python27.Functions.Examples</a>
for examples.
</p>
</html>",       revisions="<html>
<ul>
<li>
March 27, 2013, by Thierry S. Nouidui:<br/>
Added  a wrapper to <code>ModelicaFormatError</code> to support Windows OS.
</li>
<li>
January 31, 2013, by Michael Wetter:<br/>
First implementation.
</li>
</ul>
</html>"));
      end isatExchangeData;

      function isatStartCosimulation "Start the coupled simulation with ISAT"
        input String cfdFilNam "CFD input file name";
        input String[nSur] name "Surface names";
        input Modelica.SIunits.Area[nSur] A "Surface areas";
        input Modelica.SIunits.Angle[nSur] til "Surface tilt";
        input Buildings.ThermalZones.Detailed.Types.CFDBoundaryConditions[nSur] bouCon
          "Type of boundary condition";
        input Integer nPorts(min=0)
          "Number of fluid ports for the HVAC inlet and outlets";
        input String portName[nPorts]
          "Names of fluid ports as declared in the CFD input file";
        input Boolean haveSensor "Flag, true if the model has at least one sensor";
        input String sensorName[nSen]
          "Names of sensors as declared in the CFD input file";
        input Boolean haveShade "Flag, true if the windows have a shade";
        input Integer nSur "Number of surfaces";
        input Integer nSen(min=0)
          "Number of sensors that are connected to CFD output";
        input Integer nConExtWin(min=0) "number of exterior construction with window";
        input Integer nXi(min=0) "Number of independent species";
        input Integer nC(min=0) "Number of trace substances";
        input Modelica.SIunits.Density rho_start "Density at initial state";
        output Integer retVal
          "Return value of the function (0 indicates CFD successfully started.)";
      external"C" retVal = cfdStartCosimulation(
          cfdFilNam,
          name,
          A,
          til,
          bouCon,
          nPorts,
          portName,
          haveSensor,
          sensorName,
          haveShade,
          nSur,
          nSen,
          nConExtWin,
          nXi,
          nC,
          rho_start) annotation (Include="#include <cfdStartCosimulation.c>",
            IncludeDirectory="modelica://Buildings/Resources/C-Sources",
            LibraryDirectory="modelica://Buildings/Resources/Library", Library="ffd");

        annotation (Documentation(info="<html>
<p>
This function calls a C function to start the coupled simulation with CFD.</html>",
              revisions="<html>
<ul>
<li>
August 16, 2013, by Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

      end isatStartCosimulation;

      function isatSendStopCommand "Send the stop command to ISAT"

      external"C" cfdSendStopCommand() annotation (Include=
              "#include <cfdSendStopCommand.c>", IncludeDirectory=
              "modelica://Buildings/Resources/C-Sources");

        annotation (Documentation(info="<html>
<p>
This function calls a C function to send a stop command to CFD to stop the CFD simulation.</html>",
              revisions="<html>
<ul>
<li>
August 16, 2013, by Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

      end isatSendStopCommand;
    annotation (preferredView="info", Documentation(info="<html>
<p>
This package contains functions that call Python.
</p>
</html>"));
    end BaseClasses;
  annotation (preferredView="info", Documentation(info="<html>
<p>
This package contains functions that call Python.
</p>
</html>"));
  end Functions;

  package Examples "Collection of models that illustrate model use and test models"
    extends Modelica.Icons.ExamplesPackage;

    model KalmanFilter
      "Kalman filter implemented in Python and called from Modelica"
      extends Modelica.Icons.Example;
      ISAT ran(
        nDblWri=1,
        nDblRea=1,
        functionName="random",
        moduleName="KalmanFilter",
        samplePeriod=samplePeriod) "Generate a random number in Python"
        annotation (Placement(transformation(extent={{-40,0},{-20,20}})));
      Modelica.Blocks.Sources.Clock clock
        annotation (Placement(transformation(extent={{-80,0},{-60,20}})));
      ISAT kalFil(
        moduleName="KalmanFilter",
        functionName="filter",
        nDblWri=1,
        nDblRea=1,
        samplePeriod=samplePeriod) "Kalman filter in Python"
        annotation (Placement(transformation(extent={{60,20},{80,40}})));

      parameter Modelica.SIunits.Time samplePeriod=0.001
        "Sample period of component";
      Modelica.Blocks.Sources.Sine sine(freqHz=1) "Sine wave"
        annotation (Placement(transformation(extent={{-40,40},{-20,60}})));
      Modelica.Blocks.Math.Add add
        annotation (Placement(transformation(extent={{20,20},{40,40}})));
    equation
     // Delete the temporary file generated by the Python file
     // at the start and end of the simulation.
     when {initial(), terminal()} then
      Modelica.Utilities.Files.removeFile("tmp-kalman.pkl");
    end when;

      connect(clock.y, ran.uR[1]) annotation (Line(
          points={{-59,10},{-42,10}},
          color={0,0,127},
          smooth=Smooth.None));

      connect(add.y, kalFil.uR[1]) annotation (Line(
          points={{41,30},{58,30}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(ran.yR[1], add.u2) annotation (Line(
          points={{-19,10},{0,10},{0,24},{18,24}},
          color={0,0,127},
          smooth=Smooth.None));
      connect(sine.y, add.u1) annotation (Line(
          points={{-19,50},{0,50},{0,36},{18,36}},
          color={0,0,127},
          smooth=Smooth.None));
      annotation (
    experiment(Tolerance=1e-6, StopTime=1),
    __Dymola_Commands(file="modelica://Buildings/Resources/Scripts/Dymola/Utilities/IO/Python27/Examples/KalmanFilter.mos"
            "Simulate and plot"),
        Documentation(info="<html>
<p>
This example demonstrates the implementation of a Kalman filter
in Python.
The model generates a uniform random number, which is computed
in the Python file <code>KalmanFilter.py</code> by the function
<code>random(seed)</code>.
This random number is added to a sine wave and then sent to
the function <code>filter(u)</code> in the above Python file.
The function <code>filter(u)</code> implements a Kalman filter that estimates and returns
the state.
The function saves its temporary variables to a file called
<code>tmp-kalman.pkl</code>.
</p>
<p>
When simulating this model, the figure below will be generated which
shows the sine wave, the sine wave plus noise, which is input to the Kalman filter,
and the estimated state which is the output of the Kalman filter.
</p>
<p align=\"center\">
<img alt=\"image\" src=\"modelica://Buildings/Resources/Images/Utilities/IO/Python27/Examples/KalmanFilter.png\"/>
</p>
<h4>Implementation</h4>
<p>
The code is based on
<a href=\"http://www.scipy.org/Cookbook/KalmanFiltering\">
http://www.scipy.org/Cookbook/KalmanFiltering</a>.
</p>
</html>",     revisions="<html>
<ul>
<li>
February 5, 2013, by Michael Wetter:<br/>
First implementation.
</li>
</ul>
</html>"));
    end KalmanFilter;
  annotation (preferredView="info", Documentation(info="<html>
<p>
This package contains examples for the use of models that can be found in
<a href=\"modelica://Buildings.Utilities.IO.Python27\">
Buildings.Utilities.IO.Python27</a>.
</p>
</html>"));
  end Examples;
annotation (preferredView="info", Documentation(info="<html>
<p>
This package contains blocks and functions that embed Python 2.7 in Modelica.
Data can be sent to Python functions and received from Python functions.
This allows for example data analysis in Python as part of a Modelica model,
or data exchange as part of a hardware-in-the-loop simulation in which
Python is used to communicate with hardware.
</p>
<p>
See
<a href=\"modelica://Buildings.Utilities.IO.Python27.UsersGuide\">
Buildings.Utilities.IO.Python27.UsersGuide</a>
for instruction.
</p>
</html>"));
end ISAT;
