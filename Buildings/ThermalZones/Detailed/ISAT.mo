within Buildings.ThermalZones.Detailed;
model ISAT
  extends Buildings.ThermalZones.Detailed.BaseClasses.RoomHeatMassBalance(
  redeclare Buildings.ThermalZones.Detailed.BaseClasses.ISATAirHeatMassBalance air(
    final energyDynamics=energyDynamics,
    final massDynamics = massDynamics,
    final p_start=p_start,
    final T_start=T_start,
    final X_start=X_start,
    final C_start=C_start,
    final C_nominal=C_nominal,
    final mSenFac=mSenFac,
    final m_flow_nominal=m_flow_nominal,
    final homotopyInitialization=homotopyInitialization,
    final conMod=intConMod,
    final hFixed=hIntFixed,
    final use_C_flow = use_C_flow,
    final cfdFilNam = absCfdFilNam,
    final useCFD=useCFD,
    final samplePeriod=samplePeriod,
    final haveSensor=haveSensor,
    final nSen=nSen,
    final sensorName=sensorName,
    final portName=portName,
    final uSha_fixed=uSha_fixed),
    datConExt(
      each T_a_start = T_start,
      each T_b_start = T_start),
    datConExtWin(
      each T_a_start = T_start,
      each T_b_start = T_start),
    datConBou(
      each T_a_start = T_start,
      each T_b_start = T_start),
    datConPar(
      each T_a_start = T_start,
      each T_b_start = T_start));

  ////////////////////////////////////////////////////////////////////////////
  // Media declaration. This is identical to
  // Buildings.Fluid.Interfaces.LumpedVolumeDeclarations, except
  // that the comments have been changed to avoid a confusion about
  // what energyDynamics refers to.
  replaceable package Medium =
    Modelica.Media.Interfaces.PartialMedium "Medium in the component"
      annotation (choicesAllMatching = true);

  // Ports
  parameter Boolean use_C_flow=false
    "Set to true to enable input connector for trace substance that is connected to room air"
    annotation (Dialog(group="Ports"));

  // Assumptions
  parameter Modelica.Fluid.Types.Dynamics energyDynamics=Modelica.Fluid.Types.Dynamics.DynamicFreeInitial
    "Type of energy balance for zone air: dynamic (3 initialization options) or steady state"
    annotation(Evaluate=true, Dialog(tab = "Dynamics", group="Zone air"));
  parameter Modelica.Fluid.Types.Dynamics massDynamics=energyDynamics
    "Type of mass balance for zone air: dynamic (3 initialization options) or steady state"
    annotation(Evaluate=true, Dialog(tab = "Dynamics", group="Zone air"));
  final parameter Modelica.Fluid.Types.Dynamics substanceDynamics=energyDynamics
    "Type of independent mass fraction balance for zone air: dynamic (3 initialization options) or steady state"
    annotation(Evaluate=true, Dialog(tab = "Dynamics", group="Zone air"));
  final parameter Modelica.Fluid.Types.Dynamics traceDynamics=energyDynamics
    "Type of trace substance balance for zone air: dynamic (3 initialization options) or steady state"
    annotation(Evaluate=true, Dialog(tab = "Dynamics", group="Zone air"));

  parameter Real mSenFac(min=1)=1
    "Factor for scaling the sensible thermal mass of the zone air volume"
    annotation(Dialog(tab="Dynamics", group="Zone air"));

  // Initialization
  parameter Medium.AbsolutePressure p_start = Medium.p_default
    "Start value of zone air pressure"
    annotation(Dialog(tab = "Initialization"));
  parameter Medium.Temperature T_start=Medium.T_default
    "Start value of zone air temperature"
    annotation(Dialog(tab = "Initialization"));
  parameter Medium.MassFraction X_start[Medium.nX](
       quantity=Medium.substanceNames) = Medium.X_default
    "Start value of zone air mass fractions m_i/m"
    annotation (Dialog(tab="Initialization", enable=Medium.nXi > 0));
  parameter Medium.ExtraProperty C_start[Medium.nC](
       quantity=Medium.extraPropertiesNames)=fill(0, Medium.nC)
    "Start value of zone air trace substances"
    annotation (Dialog(tab="Initialization", enable=Medium.nC > 0));
  parameter Medium.ExtraProperty C_nominal[Medium.nC](
       quantity=Medium.extraPropertiesNames) = fill(1E-2, Medium.nC)
    "Nominal value of zone air trace substances. (Set to typical order of magnitude.)"
   annotation (Dialog(tab="Initialization", enable=Medium.nC > 0));

  //isat related parameters
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
  parameter String isatFilNam "CFD input file name"
    annotation (Dialog(group = "CFD",
        loadSelector(caption=
            "Select CFD input file")));
  Modelica.Blocks.Interfaces.RealOutput yISAT[nSen] if
       haveSensor "Sensor for output from ISAT" annotation (Placement(
        transformation(extent={{460,110},{480,130}}), iconTransformation(extent=
           {{200,110},{220,130}})));
protected
  final parameter String absCfdFilNam = Buildings.BoundaryConditions.WeatherData.BaseClasses.getAbsolutePath(cfdFilNam)
    "Absolute path to the CFD file";

  final parameter Boolean haveSensor = Modelica.Utilities.Strings.length(sensorName[1]) > 0
    "Flag, true if the model has at least one sensor";
  final parameter Integer nSen(min=0) = size(sensorName, 1)
    "Number of sensors that are connected to CFD output";


  ////////////////////////////////////////////////////////////////////////////
  // Input connectors
  Modelica.Blocks.Interfaces.RealInput uSha[nConExtWin](each min=0, each max=1) if
       haveShade
    "Control signal for the shading device (removed if no shade is present)"
    annotation (Placement(transformation(extent={{-300,160},{-260,200}}),
        iconTransformation(extent={{-232,164},{-200,196}})));

  Modelica.Blocks.Interfaces.RealInput C_flow[Medium.nC] if use_C_flow
    "Trace substance mass flow rate added to the room air. Enable if use_C_flow = true"
    annotation (Placement(transformation(extent={{-300,-130},{-260,-90}}),
        iconTransformation(extent={{-232,12},{-200,44}})));

equation
  connect(air.yISAT, yISAT) annotation (Line(
      points={{60,-142.5},{60,-206},{440,-206},{440,120},{470,120}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(uSha, conExtWin.uSha) annotation (Line(
      points={{-280,180},{308,180},{308,62},{281,62}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(uSha, bouConExtWin.uSha) annotation (Line(
      points={{-280,180},{308,180},{308,64},{351,64}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(uSha, conExtWinRad.uSha) annotation (Line(
      points={{-280,180},{422,180},{422,-40},{310.2,-40},{310.2,-25.6}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(irRadGai.uSha,uSha) annotation (Line(
      points={{-100.833,-22.5},{-110,-22.5},{-110,180},{-280,180}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(uSha, radTem.uSha) annotation (Line(
      points={{-280,180},{-110,180},{-110,-62},{-100.833,-62},{-100.833,-62.5}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(uSha, shaSig.u) annotation (Line(
      points={{-280,180},{-248,180},{-248,160},{-222,160}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(air.uSha,uSha)  annotation (Line(
      points={{39.6,-120},{8,-120},{8,180},{-280,180}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(C_flow, air.C_flow) annotation (Line(points={{-280,-110},{-200,-110},{
          -200,-114},{-200,-114},{-200,-202},{-18,-202},{-18,-141},{39,-141}},
        color={0,0,127}));
  annotation (
    Documentation(info="<html>
<p>
This model is a mixed air room model with an isat model, which returns yISAT[]
with inputs based on a regression isat model.
This model is derived from MixedAir, which was first implemented by Michael Wetter.
</p>
</html>", revisions="<html>
<ul>
<li>
April 23, 2019, by Xu Han, Wangda Zuo:<br/>
First implementation.
</li>
</ul>
</html>"));
end ISAT;
