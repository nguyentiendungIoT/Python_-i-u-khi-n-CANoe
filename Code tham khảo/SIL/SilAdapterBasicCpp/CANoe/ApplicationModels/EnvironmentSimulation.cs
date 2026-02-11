using System;
using Vector.Tools;
using Vector.CANoe.Runtime;
using Vector.CANoe.Sockets;
using Vector.CANoe.Threading;
using Vector.Diagnostics;
using NetworkDB;
using NetworkDB._SystemDataTypes;

using IServiceProvider = Vector.CANoe.Runtime.IServiceProvider;


public class EnvironmentSimulation : MeasurementScript
{
    // Counter for how many (timer) steps the average temperature is increased / decreased
    private static readonly int INCREASE_DECREASE_TEMP_COUNTER = 20;

    private int _increaseDecreaseTemperatureCounter;
    private bool _decreaseTemperature;
    private Timer _timer;
    private Random _rng;

    /// <summary>
    /// Called before measurement start to perform necessary initializations,
    /// e.g. to create objects. During measurement, few additional objects
    /// should be created to prevent garbage collection runs in time-critical
    /// simulations.
    /// </summary>
    public override void Initialize()
    {
      _increaseDecreaseTemperatureCounter = INCREASE_DECREASE_TEMP_COUNTER;
      _decreaseTemperature = true;
      _timer = new Timer(TimeSpan.FromMilliseconds(100), (sender, args) => IncreaseDecreaseTemperature());
      _rng = new Random();
    }
    
    /// <summary>Notification that the measurement starts.</summary>
    public override void Start()
    {
    
    }
    
    /// <summary>Notification that the measurement ends.</summary>
    public override void Stop()
    {
    
    }
    
    /// <summary>
    /// Cleanup after the measurement. Complement to Initialize. This is not
    /// a "Dispose" method; your object should still be usable afterwards.
    /// </summary>
    public override void Shutdown()
    {
    
    }

    [OnChange(Panel.Panel.MemberIDs.EnvironmentSimulationActive)]
    public void OnAutomaticModeChange()
    {
      if (Panel.Panel.EnvironmentSimulationActive.Value)
      {
        RoomTemperatureControl.Sensor1.Temperature.Value = 23.0;
        RoomTemperatureControl.Sensor2.Temperature.Value = 23.0;
        RoomTemperatureControl.Sensor3.Temperature.Value = 23.0;
        _timer.Start();
      }
      else
      {
        _timer.Stop();
      }
    }

    private void IncreaseDecreaseTemperature()
    {
        
      // If counter is still > 0, stay with increasing / decreasing temperature
      if (_increaseDecreaseTemperatureCounter > 0)
      {
        _increaseDecreaseTemperatureCounter -= 1;
      }
      else
      {
        // Counter for increasing / decreasing time interval / steps is 0, so
        // change from increasing to decreasing temperature or the other way around
        _decreaseTemperature = !_decreaseTemperature;
        _increaseDecreaseTemperatureCounter = INCREASE_DECREASE_TEMP_COUNTER;
      }

      var temperatureChange = _decreaseTemperature ? -1 : 1;
      var sensorNumber = _rng.Next(minValue: 1, maxValue: 4);
      switch (sensorNumber)
      {
        case 1:
          RoomTemperatureControl.Sensor1.Temperature.Value += temperatureChange;
          break;
        case 2:
          RoomTemperatureControl.Sensor2.Temperature.Value += temperatureChange;
          break;
        default:
          RoomTemperatureControl.Sensor3.Temperature.Value += temperatureChange;
          break;
      }
    }
}