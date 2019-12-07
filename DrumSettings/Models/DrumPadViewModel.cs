using System;
using System.Reactive.Linq;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using ReactiveUI;

namespace Arkserg.TeensyDrumModule.DrumSettings.Models
{
    public class DrumPadViewModel : ReactiveObject
    {
        private DrumType? _type;
        private byte _id;
        private string _name;
        private byte _channel;

        private bool _initialized;

        public DrumPadViewModel()
        {
            Changed.Throttle(TimeSpan.FromMilliseconds(500)).Subscribe(events => HandleModelChanged());
        }

        private void HandleModelChanged()
        {
            if (!_initialized)
                _initialized = true;
            else
                OnModelChanged?.Invoke(this);
        }

        public event Action<DrumPadViewModel> OnModelChanged;

        public DrumType? Type
        {
            get => _type;
            set => this.RaiseAndSetIfChanged(ref _type, value);            
        }

        public byte Id
        {
            get => _id;
            set => this.RaiseAndSetIfChanged(ref _id, value);
        }

        public string Name
        {
            get => _name;
            set => this.RaiseAndSetIfChanged(ref _name, value);
        }

        public byte Channel
        {
            get => _channel;
            set => this.RaiseAndSetIfChanged(ref _channel, value);
        }

        public override string ToString()
        {
            return Name;
        }
    }

    public class SinglePiezoPadViewModel : DrumPadViewModel
    {
        private short _thresholdMin;
        private short _thresholdMax;
        private byte _sensorScantime;
        private byte _sensorMasktime;
        private byte _padNote;

        public Int16 ThresholdMin
        {
            get => _thresholdMin;
            set => this.RaiseAndSetIfChanged(ref _thresholdMin, value);
        }

        public Int16 ThresholdMax
        {
            get => _thresholdMax;
            set => this.RaiseAndSetIfChanged(ref _thresholdMax, value);
        }

        public byte SensorScantime
        {
            get => _sensorScantime;
            set => this.RaiseAndSetIfChanged(ref _sensorScantime, value);
        }

        public byte SensorMasktime
        {
            get => _sensorMasktime;
            set => this.RaiseAndSetIfChanged(ref _sensorMasktime, value);
        }

        public byte PadNote
        {
            get => _padNote;
            set => this.RaiseAndSetIfChanged(ref _padNote, value);
        }
    }

    public class YamahaPcyCymbalViewModel : SinglePiezoPadViewModel
    {
        private byte _zoneSensorChannel;
        private byte _bellNote;
        private byte _edgeNote;

        public byte ZoneSensorChannel
        {
            get => _zoneSensorChannel;
            set => this.RaiseAndSetIfChanged(ref _zoneSensorChannel, value);
        }

        public byte BellNote
        {
            get => _bellNote;
            set => this.RaiseAndSetIfChanged(ref _bellNote, value);
        }

        public byte EdgeNote
        {
            get => _edgeNote;
            set => this.RaiseAndSetIfChanged(ref _edgeNote, value);
        }
    }
}
