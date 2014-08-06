within Buildings.Electrical.AC.ThreePhasesUnbalanced.Sensors;
model GeneralizedSensor4

  Interfaces.Terminal4_n terminal_n
    annotation (Placement(transformation(extent={{-110,-10},{-90,10}})));
  Interfaces.Terminal4_p terminal_p
    annotation (Placement(transformation(extent={{90,-10},{110,10}})));
  Modelica.Blocks.Interfaces.RealOutput V[4](final quantity="ElectricPotential",
                                          final unit="V") "Voltage"           annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={10,-40}),  iconTransformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={0,-90})));
  Modelica.Blocks.Interfaces.RealOutput I[4](final quantity="ElectricCurrent",
                                          final unit="A") "Current"           annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={70,-40}), iconTransformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={60,-90})));
  Modelica.Blocks.Interfaces.RealOutput S[4,terminal_n.phase[1].PhaseSystem.n](
                                          final quantity="Power",
                                          final unit="W") "Phase powers"             annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={-50,-40}),iconTransformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={-60,-90})));
equation

  for i in 1:4 loop
    V[i]   = Buildings.Electrical.PhaseSystems.OnePhase.systemVoltage(terminal_n.phase[i].v);
    I[i]   = Buildings.Electrical.PhaseSystems.OnePhase.systemCurrent(terminal_n.phase[i].i);
    S[i,:] = Buildings.Electrical.PhaseSystems.OnePhase.phasePowers_vi(v=terminal_n.phase[i].v, i=terminal_n.phase[i].i);
  end for;

  connect(terminal_n, terminal_p) annotation (Line(
      points={{-100,0},{100,0}},
      color={0,120,120},
      smooth=Smooth.None));
  annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
            -100},{100,100}}), graphics), Icon(graphics={
        Rectangle(
          extent={{-70,28},{70,-30}},
          lineColor={0,0,0},
          fillColor={255,255,255},
          fillPattern=FillPattern.Solid),
        Ellipse(
          extent={{-60,60},{60,-60}},
          lineColor={0,0,0},
          fillColor={255,255,255},
          fillPattern=FillPattern.Solid),
        Line(
          points={{-92,0},{-70,0}},
          color={0,0,0},
          smooth=Smooth.None),
        Text(
          extent={{-120,-42},{0,-82}},
          lineColor={0,0,0},
          lineThickness=1,
          fillColor={255,255,255},
          fillPattern=FillPattern.Solid,
          textString="S"),
        Polygon(
          points={{-0.48,33.6},{18,28},{18,59.2},{-0.48,33.6}},
          lineColor={0,0,0},
          fillColor={0,0,0},
          fillPattern=FillPattern.Solid),
        Line(points={{-37.6,15.7},{-54,22}},     color={0,0,0}),
        Line(points={{-22.9,34.8},{-32,50}},     color={0,0,0}),
        Line(points={{0,60},{0,42}}, color={0,0,0}),
        Line(points={{22.9,34.8},{32,50}},     color={0,0,0}),
        Line(points={{37.6,15.7},{54,24}},     color={0,0,0}),
        Line(points={{0,2},{9.02,30.6}}, color={0,0,0}),
        Ellipse(
          extent={{-5,7},{5,-3}},
          lineColor={0,0,0},
          fillColor={0,0,0},
          fillPattern=FillPattern.Solid),
        Line(
          points={{70,0},{92,0}},
          color={0,0,0},
          smooth=Smooth.None),
        Text(
          extent={{-60,-42},{60,-82}},
          lineColor={0,0,0},
          lineThickness=1,
          fillColor={255,255,255},
          fillPattern=FillPattern.Solid,
          textString="V"),
        Text(
          extent={{0,-40},{120,-80}},
          lineColor={0,0,0},
          lineThickness=1,
          fillColor={255,255,255},
          fillPattern=FillPattern.Solid,
          textString="I")}));
end GeneralizedSensor4;