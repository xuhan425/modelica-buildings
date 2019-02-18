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

  package BaseClasses "Package with functions that call ISAT"
    extends Modelica.Icons.BasesPackage;

    function isatExchangeData "Function that communicates with ISAT"
      input Real[nU] u "Input to ISAT";
      input Integer nU "Number of inputs to ISAT";
      input Integer nY "Number of outputs from ISAT";
      output Real[nY] y "Output computed by ISAT";
      output Integer retVal "Return value for ISAT simulation status";
    external"C" retVal = cfdExchangeData(
        u,
        nU,
        nY,
        modTimRea,
        y) annotation (Include="#include <isatExchangeData.c>", IncludeDirectory=
            "modelica://Buildings/Resources/C-Sources");

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
</html>",     revisions="<html>
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
      input String isatCasPath "isat case path";
      input Integer flaApp "flag of application for isat (0: room; 1: data center)";
      input Integer flaTri "flag of train or evaluate isat (0: evaluate; 1: train)";
      input Real err "define error tolerance for isat case";
      output Integer retVal
        "Return value of the function (0 indicates ISAT successfully started.)";
    external"C" retVal = isatStartCosimulation(
        isatCasPath,
        flaApp,
        flaTri,
        err) annotation (Include="#include <isatStartCosimulation.c>",
          IncludeDirectory="modelica://Buildings/Resources/C-Sources",
          LibraryDirectory="modelica://Buildings/Resources/Library", Library="isat");

      annotation (Documentation(info="<html>
<p>
This function calls a C function to start the coupled simulation with ISAT.</html>",
            revisions="<html>
<ul>
<li>
February 18, 2019, by Xu Han, Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));

    end isatStartCosimulation;

    function isatSendStopCommand "Send the stop command to ISAT"

    external"C" isatSendStopCommand() annotation (Include=
            "#include <isatSendStopCommand.c>", IncludeDirectory=
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

  package Examples "Collection of models that illustrate model use and test models"
    extends Modelica.Icons.ExamplesPackage;

    model DataCenter
      "A data center case trained by ISAT and called from Modelica"
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
    end DataCenter;

    model Room "A room case trained by ISAT and called from Modelica"
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
    end Room;
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
