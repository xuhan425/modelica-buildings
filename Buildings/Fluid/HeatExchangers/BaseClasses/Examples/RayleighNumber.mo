within Buildings.Fluid.HeatExchangers.BaseClasses.Examples;
model RayleighNumber "Test model for RayleighNumber"
  import Buildings;
  extends Modelica.Icons.Example;
  Buildings.Fluid.HeatExchangers.BaseClasses.RayleighNumber rayleighNumber(ChaLen=
        0.1, redeclare package Medium =
        Buildings.Media.ConstantPropertyLiquidWater)
    annotation (Placement(transformation(extent={{-10,0},{14,24}})));
  Modelica.Blocks.Sources.Constant TSur(k=350) "Temperature of the hot surface"
    annotation (Placement(transformation(extent={{-74,10},{-54,30}})));
  Modelica.Blocks.Sources.Constant TFlu(k=350)
    "Temperature of the surrounding fluid"
    annotation (Placement(transformation(extent={{-74,-28},{-54,-8}})));
equation
  connect(TSur.y, rayleighNumber.TSur)  annotation (Line(
      points={{-53,20},{-32,20},{-32,16.8},{-12.4,16.8}},
      color={0,0,127},
      smooth=Smooth.None));
  connect(TFlu.y, rayleighNumber.TFlu)   annotation (Line(
      points={{-53,-18},{-32,-18},{-32,6.96},{-12.4,6.96}},
      color={0,0,127},
      smooth=Smooth.None));
  annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
            -100},{100,100}}), graphics), Commands(file=
          "Resources/Scripts/Dymola/Fluid/HeatExchangers/BaseClasses/Examples/RayleighNumber.mos"
        "Simulate and Plot"),
        Documentation(info="<html>
        <p>
        Test model for <a href=\"modelica://Buildings.Fluid.HeatExchangers.BaseClasses.RayleighNumber\">
        Buildings.Fluid.HeatExchangers.BaseClasses.RayleighNumber</a>.
        </p>
        </html>"));
end RayleighNumber;
