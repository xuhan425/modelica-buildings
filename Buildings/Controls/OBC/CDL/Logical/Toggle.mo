within Buildings.Controls.OBC.CDL.Logical;
block Toggle "Toggles output value whenever its input turns true"

  parameter Boolean pre_y_start=false "Start value of pre(y) if clr=false";

  Interfaces.BooleanInput u "Toggle input"
    annotation (Placement(transformation(extent={{-120,-10},{-100,10}})));
  Interfaces.BooleanInput clr "Clear input"
    annotation (Placement(transformation(extent={{-120,-70},{-100,-50}})));
  Interfaces.BooleanOutput y "Output signal"
    annotation (Placement(transformation(extent={{100,-10},{120,10}})));

protected
  Integer scenario "Scenario index";

initial equation
  pre(y) = pre_y_start;
  pre(u) = false;
  pre(clr) = false;
  pre(scenario) = 0;

equation
  when initial() then
    scenario = 1;
  elsewhen (not clr) and ((pre(u) <> u) and (pre(u) == false)) and (pre(y) == false) then
    scenario = 2;
  elsewhen (not clr) and ((pre(u) <> u) and (pre(u) == false)) and (pre(y) == true) then
    scenario = 3;
  elsewhen (not clr) and ((pre(u) <> u) and (pre(u) == true)) then
    scenario = 4;
  elsewhen clr then
    scenario = 5;
  end when;

  if clr then
    y = false;
  else
    if (scenario == 1) then y = if clr then false else pre(y);
    elseif (scenario == 2) then y = true;
    elseif (scenario == 3) then y = false;
    elseif (scenario == 4) then y = pre(y);
    else y = false;
    end if;
  end if;

annotation (defaultComponentName="tog",
  Icon(coordinateSystem(preserveAspectRatio=true, extent={{-100,
            -100},{100,100}}), graphics={Rectangle(
          extent={{-100,100},{100,-100}},
          fillColor={210,210,210},
          lineThickness=5.0,
          fillPattern=FillPattern.Solid,
          borderPattern=BorderPattern.Raised),
        Ellipse(
          extent={{-73,9},{-87,-5}},
          lineColor=DynamicSelect({235,235,235}, if u then {0,255,0}
               else {235,235,235}),
          fillColor=DynamicSelect({235,235,235}, if u then {0,255,0}
               else {235,235,235}),
          fillPattern=FillPattern.Solid),
        Ellipse(
          extent={{-73,-53},{-87,-67}},
          lineColor=DynamicSelect({235,235,235}, if u0 then {0,255,0}
               else {235,235,235}),
          fillColor=DynamicSelect({235,235,235}, if u0 then {0,255,0}
               else {235,235,235}),
          fillPattern=FillPattern.Solid),
        Line(points={{-68,-62},{4,-62},{4,-22},{74,-22}}, color={255,0,255}),
        Line(points={{-68,24},{-48,24},{-48,56},{-16,56},{-16,24},{24,24},{24,56},
              {54,56},{54,24},{74,24}}, color={255,0,255}),
        Text(
          extent={{-22,72},{28,58}},
          lineColor={0,0,0},
          fillColor={210,210,210},
          fillPattern=FillPattern.Solid,
          textString="Toggle input"),
        Text(
          extent={{-150,150},{150,110}},
          lineColor={0,0,255},
          textString="%name"),
        Ellipse(
          extent={{71,7},{85,-7}},
          lineColor=DynamicSelect({235,235,235}, if y then {0,255,0}
               else {235,235,235}),
          fillColor=DynamicSelect({235,235,235}, if y then {0,255,0}
               else {235,235,235}),
          fillPattern=FillPattern.Solid),
        Text(
          extent={{-14,-10},{14,-20}},
          lineColor={0,0,0},
          fillColor={210,210,210},
          fillPattern=FillPattern.Solid,
          textString="Clear")}),
  Documentation(info="<html>
<p>
Block that generates a <code>true</code> output when toggle input <code>u</code> 
rises from <code>false</code> to <code>true</code>, provided that the clear input
<code>clr</code> is <code>false</code> or also became at the same time 
<code>false</code>. The output remains <code>true</code> until
</p>
<ul>
<li>
the toggle input <code>u</code> rises from <code>false</code> to <code>true</code> again, or
</li>
<li>
the clear input <code>clr</code> rises from <code>false</code> to <code>true</code>.
</li>
</ul>
<p>
If the clear input <code>clr</code> is <code>true</code>, the output <code>y</code>
switches to <code>false</code> (if it was <code>true</code>) and it remains <code>false</code>,
regardless of the value of the toggle input <code>u</code>.
</p>

<p>
At initial time, if <code>clr = false</code>, then the output will be 
<code>y = pre_y_start</code>. Otherwise it will be <code>y=false</code> 
(because the clear input <code>clr</code> is <code>true</code>).
</p>

<p align=\"center\">
<img src=\"modelica://Buildings/Resources/Images/Controls/OBC/CDL/Logical/Toggle.png\"
     alt=\"Toggle.png\" />
</p>

</html>", revisions="<html>
<ul>
<li>
April 4, 2019, by Jianjun Hu:<br/>
Corrected implementation that causes wrong output at initial stage. 
This is for <a href=\"https://github.com/ibpsa/modelica-ibpsa/issues/1402\">issue 1402</a>.
</li>
<li>
March 31, 2017, by Jianjun Hu:<br/>
First implementation.
</li>
</ul>
</html>"));
end Toggle;
