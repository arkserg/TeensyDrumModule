using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reactive.Linq;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using ReactiveUI;

namespace Arkserg.TeensyDrumModule.DrumSettings.Models
{
    public class DrumPadViewModel : ReactiveObject
    {
        private DrumType _type;
        private byte _channel;
        private string _name;
        private bool _enabled;

        private bool _initialized;
        private bool _typeChanged = false;

        public DrumPadViewModel()
        {
            Changed.Throttle(TimeSpan.FromMilliseconds(500)).Subscribe(events => HandleModelChanged());
            PropertyChanged += HandlePropertyChanged;
        }

        private void HandleModelChanged()
        {
            if (!_initialized)
                _initialized = true;
            else if (_typeChanged)
                _typeChanged = false;
            else
                OnModelChanged?.Invoke(this);
        }

        private void HandlePropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (_initialized && e.PropertyName == nameof(Type))
            {
                _typeChanged = true;
                OnDrumTypeChanged?.Invoke(this);
            }
        }

        public event Action<DrumPadViewModel> OnModelChanged;
        public event Action<DrumPadViewModel> OnDrumTypeChanged;

        public DrumType Type
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

        public List<DrumType> AvailableTypes
        {
            get
            {
                var result = new List<DrumType> { DrumType.EmptyDrumPad, DrumType.SinglePiezoPad };

                if (Channel < 4)
                    result.Add(DrumType.DualPiezoPad);
                else
                    result.AddRange(new List<DrumType>{ DrumType.DualZoneCymbal, DrumType.ThreeZoneCymbal, DrumType.HiHatController});
                return result;
            }
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