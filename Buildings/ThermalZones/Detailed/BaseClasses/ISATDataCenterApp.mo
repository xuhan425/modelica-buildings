within Buildings.ThermalZones.Detailed.BaseClasses;
model ISATDataCenterApp "Appication for data center"

  parameter Integer nRacks=2 "Number of racks";
  parameter Integer nOutputs=1 "Number of outputs";

  ISATExchange iSATExchange
    annotation (Placement(transformation(extent={{-20,0},{0,20}})));
  Modelica.Blocks.Interfaces.RealInput u[nRacks](final unit="kW")
    annotation (Placement(transformation(extent={{-140,-10},{-100,30}})));
  Modelica.Blocks.Interfaces.RealOutput y[nOutputs](final unit="K")
    annotation (Placement(transformation(extent={{100,0},{120,20}})));
equation

   for i in 1:nRacks loop
     connect(u[i], iSATExchange.u[i])
     annotation (Line(points={{-120,10},{-22,10}}, color={0,0,127}));
   end for;

   for i in 1:nOutputs loop
     connect(iSATExchange.y[i], y[i])
     annotation (Line(points={{1,10},{110,10}}, color={0,0,127}));
   end for;

end ISATDataCenterApp;
