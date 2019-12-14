using System;
using System.Reactive.Linq;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using ReactiveUI;

namespace Arkserg.TeensyDrumModule.DrumSettings.Models
{
    public class DrumPadViewModel : ReactiveObject
    {
        private DrumType? _type;
        private byte _channel;
        private string _name;
        private bool _enabled;

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

        public byte Channel
        {
            get => _channel;
            set => this.RaiseAndSetIfChanged(ref _channel, value);
        }

        public string Name
        {
            get => _name;
            set => this.RaiseAndSetIfChanged(ref _name, value);
        }

        public bool Enabled
        {
            get => _enabled;
            set => this.RaiseAndSetIfChanged(ref _enabled, value);
        }

        public override string ToString()
        {
            return $"{_channel} {_name}";
        }
    }

    public class EmptyDrumPadViewModel : DrumPadViewModel
    {
    }

    public class HiHatControllerViewModel : DrumPadViewModel
    {
        private byte _ccControl;
        private short _thresholdMin;
        private short _thresholdMax;
        
        public byte CcControl
        {
            get => _ccControl;
            set => this.RaiseAndSetIfChanged(ref _ccControl, value);
        }

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
    }

    public class SinglePiezoPadViewModel : DrumPadViewModel
    {
        private byte _padNote;
        private short _thresholdMin;
        private short _thresholdMax;
        private byte _sensorScantime;
        private byte _sensorMasktime;
        private byte _amplification;
        
        public byte PadNote
        {
            get => _padNote;
            set => this.RaiseAndSetIfChanged(ref _padNote, value);
        }

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

        public byte Amplification
        {
            get => _amplification;
            set => this.RaiseAndSetIfChanged(ref _amplification, value);
        }
    }

    public class DualZoneCymbalViewModel : SinglePiezoPadViewModel
    {
        private byte _edgeNote;

        public byte EdgeNote
        {
            get => _edgeNote;
            set => this.RaiseAndSetIfChanged(ref _edgeNote, value);
        }
    }

    public class ThreeZoneCymbalViewModel : DualZoneCymbalViewModel
    {
        private byte _bellNote;

        public byte BellNote
        {
            get => _bellNote;
            set => this.RaiseAndSetIfChanged(ref _bellNote, value);
        }
    }
}