within Buildings.ThermalZones.Detailed;
model ISAT "Model of a room in which the air is computed using ISAT"
  extends Buildings.ThermalZones.Detailed.BaseClasses.RoomHeatMassBalance(
  redeclare BaseClasses.CFDAirHeatMassBalance air(
    final massDynamics = massDynamics,
    final cfdFilNam = absCfdFilNam,
    final useCFD=useCFD,
    final samplePeriod=samplePeriod,
    final haveSensor=haveSensor,
    final nSen=nSen,
    final sensorName=sensorName,
    final portName=portName,
    final uSha_fixed=uSha_fixed,
    final p_start=p_start));

  // Assumptions
  parameter Modelica.Fluid.Types.Dynamics massDynamics=Modelica.Fluid.Types.Dynamics.DynamicFreeInitial
    "Formulation of mass balance"
    annotation(Evaluate=true, Dialog(tab = "Dynamics", group="Equations"));
  // Initialization
  parameter Medium.AbsolutePressure p_start = Medium.p_default
    "Start value of pressure"
    annotation(Dialog(tab = "Initialization"));

  parameter Boolean useCFD = true
    "Set to false to deactivate the CFD computation and use instead yFixed as output"
    annotation(Dialog(group = "CFD"), Evaluate = true);
  parameter Modelica.SIunits.Time samplePeriod(min=100*Modelica.Constants.eps)
    "Sample period of component"
    annotation(Dialog(group = "Sampling"));
  parameter Real uSha_fixed[nConExtWin] = zeros(nConExtWin)
    "Constant control signal for the shading device (0: unshaded; 1: fully shaded)";
  parameter String sensorName[:]
    "Names of sensors as declared in the CFD input file"
    annotation(Dialog(group = "CFD"));
  parameter String portName[nPorts] = {"port_" + String(i) for i in 1:nPorts}
    "Names of fluid ports as declared in the CFD input file"
    annotation(Dialog(group = "CFD"));
  parameter String cfdFilNam "CFD input file name"
    annotation (Dialog(group = "CFD",
        loadSelector(caption=
            "Select CFD input file")));
  Modelica.Blocks.Interfaces.RealOutput yCFD[nSen] if
       haveSensor "Sensor for output from CFD"
    annotation (Placement(transformation(
     extent={{460,110},{480,130}}), iconTransformation(extent={{200,110},{220, 130}})));
protected
  final parameter String absCfdFilNam = Buildings.BoundaryConditions.WeatherData.BaseClasses.getAbsolutePath(cfdFilNam)
    "Absolute path to the CFD file";

  final parameter Boolean haveSensor = Modelica.Utilities.Strings.length(sensorName[1]) > 0
    "Flag, true if the model has at least one sensor";
  final parameter Integer nSen(min=0) = size(sensorName, 1)
    "Number of sensors that are connected to CFD output";
  Modelica.Blocks.Sources.Constant conSha[nConExtWin](final k=uSha_fixed) if
       haveShade "Constant signal for shade"
    annotation (Placement(transformation(extent={{-260,170},{-240,190}})));

equation
  connect(air.yCFD, yCFD) annotation (Line(
      points={{61,-142.5},{61,-206},{440,-206},{440,120},{470,120}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(conSha.y, conExtWin.uSha) annotation (Line(
      points={{-239,180},{328,180},{328,62},{281,62}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(conSha.y, bouConExtWin.uSha) annotation (Line(
      points={{-239,180},{328,180},{328,64},{351,64}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(conSha.y, conExtWinRad.uSha) annotation (Line(
      points={{-239,180},{420,180},{420,-42},{310.2,-42},{310.2,-25.6}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(irRadGai.uSha,conSha.y)
                             annotation (Line(
      points={{-100.833,-22.5},{-112,-22.5},{-112,180},{-239,180}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(conSha.y, radTem.uSha) annotation (Line(
      points={{-239,180},{-112,180},{-112,-62},{-100.833,-62},{-100.833,-62.5}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(conSha.y, shaSig.u) annotation (Line(
      points={{-239,180},{-228,180},{-228,160},{-222,160}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(air.uSha,conSha.y)  annotation (Line(
      points={{39.6,-120},{28,-120},{28,180},{-239,180}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (
  Icon(coordinateSystem(preserveAspectRatio=false, extent={{-200,-200},{200,200}}),
                               graphics={Rectangle(
          extent={{-140,138},{140,78}},
          pattern=LinePattern.None,
          fillColor={255,0,0},
          fillPattern=FillPattern.Solid),
        Bitmap(
          extent={{-140,-170},{142,166}},
          imageSource=
              "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",

          fileName=
              "modelica://Buildings/Resources/Images/ThermalZones/Detailed/isat.png"),
        Text(
          extent={{160,100},{200,140}},
          lineColor={0,0,127},
          fontSize=20,
          textStyle={TextStyle.Bold},
          textString="yISAT"),
        Text(
          extent={{-86,-14},{0,16}},
          lineColor={0,0,0},
          textString="air"),
        Text(
          extent={{-102,-50},{-22,-26}},
          lineColor={0,0,0},
          textString="radiation"),
        Text(
          extent={{-114,-134},{-36,-116}},
          lineColor={0,0,0},
          textString="surface")}),
    Documentation(info="<html>
<p>
Room model that computes the room air flow using computational fluid dynamics (CFD). The CFD simulation is coupled to the thermal simulation of the room
and, through the fluid port, to the air conditioning system.
</p>
<p>
Currently, the supported CFD program is the
Fast Fluid Dynamics (FFD) program <a href=\"#ZUO2010\">(Zuo 2010)</a>.
See
<a href=\"modelica://Buildings.ThermalZones.Detailed.UsersGuide\">Buildings.ThermalZones.Detailed.UsersGuide</a>
for detailed explanations.
</p>
<h4>References</h4>
<p>
<a name=\"ZUO2010\"/>
Wangda Zuo. <a href=\"http://docs.lib.purdue.edu/dissertations/AAI3413824/\">
Advanced simulations of air distributions in buildings</a>.
Ph.D. Thesis, School of Mechanical Engineering, Purdue University, 2010.
</p>
</html>",
revisions="<html>
<ul>
<li>
May 2, 2016, by Michael Wetter:<br/>
Refactored implementation of latent heat gain.
This is for
<a href=\"https://github.com/lbl-srg/modelica-buildings/issues/515\">issue 515</a>.
</li>
<li>
April 21, 2016, by Michael Wetter:<br/>
Added parameter <code>absCfdFilNam</code> as the call to
<a href=\"modelica://Buildings.BoundaryConditions.WeatherData.BaseClasses.getAbsolutePath\">
Buildings.BoundaryConditions.WeatherData.BaseClasses.getAbsolutePath</a>
was removed from
<a href=\"modelica://Buildings.ThermalZones.Detailed.BaseClasses.CFDExchange\">
Buildings.ThermalZones.Detailed.BaseClasses.CFDExchange</a>.
This is for
<a href=\"https://github.com/lbl-srg/modelica-buildings/issues/506\">Buildings, #506</a>.
</li>
<li>
August 1, 2013, by Michael Wetter and Wangda Zuo:<br/>
First Implementation.
</li>
</ul>
</html>"),
    Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-260,-220},{460,
            200}})));
end ISAT;
