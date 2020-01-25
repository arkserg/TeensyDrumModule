using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reactive.Linq;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using ReactiveUI;
using System.Linq;

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

        public List<ScaleType> ScaleTypes => Enum.GetValues(typeof(ScaleType)).Cast<ScaleType>().ToList();

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
        private Int16 _scan;
        private Int16 _hold;
        private Int16 _decay;
        private byte _gain;
        private ScaleType _scale;
        private byte _lift;
        private bool _xTalkCancellation;
        private Int16 _xTalkFactor;

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

        public Int16 Scan
        {
            get => _scan;
            set => this.RaiseAndSetIfChanged(ref _scan, value);
        }

        public Int16 Hold
        {
            get => _hold;
            set => this.RaiseAndSetIfChanged(ref _hold, value);
        }

        public Int16 Decay
        {
            get => _decay;
            set => this.RaiseAndSetIfChanged(ref _decay, value);
        }

        public byte Gain
        {
            get => _gain;
            set => this.RaiseAndSetIfChanged(ref _gain, value);
        }

        public ScaleType Scale
        {
            get => _scale;
            set => this.RaiseAndSetIfChanged(ref _scale, value);
        }

        public byte Lift
        {
            get => _lift;
            set => this.RaiseAndSetIfChanged(ref _lift, value);
        }

        public bool XTalkCancellation
        {
            get => _xTalkCancellation;
            set => this.RaiseAndSetIfChanged(ref _xTalkCancellation, value);
        }

        public Int16 XTalkFactor
        {
            get => _xTalkFactor;
            set => this.RaiseAndSetIfChanged(ref _xTalkFactor, value);
        }
    }

    public class DualPiezoPadViewModel : SinglePiezoPadViewModel
    {
        private byte _sideClickNote;
        private byte _rimShotNote;
        private Int16 _secondZoneThresholdMin;
        private Int16 _secondZoneThresholdMax;
        private byte _secondZoneScanTime;
        private byte _secondZoneMaskTime;
        private byte _secondZoneAmplification;
        private byte _rimShotBoundLeft;
        private byte _rimShotBoundRight;

        public byte SideClickNote
        {
            get => _sideClickNote;
            set => this.RaiseAndSetIfChanged(ref _sideClickNote, value);
        }

        public byte RimShotNote
        {
            get => _rimShotNote;
            set => this.RaiseAndSetIfChanged(ref _rimShotNote, value);
        }

        public Int16 SecondZoneThresholdMin
        {
            get => _secondZoneThresholdMin;
            set => this.RaiseAndSetIfChanged(ref _secondZoneThresholdMin, value);
        }

        public Int16 SecondZoneThresholdMax
        {
            get => _secondZoneThresholdMax;
            set => this.RaiseAndSetIfChanged(ref _secondZoneThresholdMax, value);
        }

        public byte SecondZoneScanTime
        {
            get => _secondZoneScanTime;
            set => this.RaiseAndSetIfChanged(ref _secondZoneScanTime, value);
        }

        public byte SecondZoneMaskTime
        {
            get => _secondZoneMaskTime;
            set => this.RaiseAndSetIfChanged(ref _secondZoneMaskTime, value);
        }

        public byte SecondZoneAmplification
        {
            get => _secondZoneAmplification;
            set => this.RaiseAndSetIfChanged(ref _secondZoneAmplification, value);
        }

        public byte RimShotBoundLeft
        {
            get => _rimShotBoundLeft;
            set => this.RaiseAndSetIfChanged(ref _rimShotBoundLeft, value);
        }

        public byte RimShotBoundRight
        {
            get => _rimShotBoundRight;
            set => this.RaiseAndSetIfChanged(ref _rimShotBoundRight, value);
        }
    }

    public class DualZoneCymbalViewModel : SinglePiezoPadViewModel
    {
        private byte _edgeNote;
        private bool _chokeEnabled;
        private byte _chokeTimeThreshold;

        public byte EdgeNote
        {
            get => _edgeNote;
            set => this.RaiseAndSetIfChanged(ref _edgeNote, value);
        }

        public bool ChokeEnabled
        {
            get => _chokeEnabled;
            set => this.RaiseAndSetIfChanged(ref _chokeEnabled, value);
        }

        public byte ChokeTimeThreshold
        {
            get => _chokeTimeThreshold;
            set => this.RaiseAndSetIfChanged(ref _chokeTimeThreshold, value);
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