using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using Arkserg.TeensyDrumModule.DrumModuleLibrary;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels;
using Arkserg.TeensyDrumModule.DrumSettings.Models;
using AutoMapper;
using System.Threading.Tasks;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;

namespace Arkserg.TeensyDrumModule.DrumSettings
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, IDisposable
    {
        public ObservableCollection<DrumPadViewModel> Drums { get; set; }

        private readonly IMapper _mapper;
        private DrumModuleConnection _connection;

        public MainWindow()
        {
            InitializeComponent();

            _mapper = new MapperConfiguration(config =>
            {
                config.CreateMap<EmptyDrumPad, EmptyDrumPadViewModel>();
                config.CreateMap<EmptyDrumPadViewModel, EmptyDrumPad>();

                config.CreateMap<DualZoneCymbal, DualZoneCymbalViewModel>();
                config.CreateMap<DualZoneCymbalViewModel, DualZoneCymbal>();

                config.CreateMap<HiHatController, HiHatControllerViewModel>();
                config.CreateMap<HiHatControllerViewModel, HiHatController>();

                config.CreateMap<SinglePiezoPad, SinglePiezoPadViewModel>();
                config.CreateMap<SinglePiezoPadViewModel, SinglePiezoPad>();

                config.CreateMap<ThreeZoneCymbal, ThreeZoneCymbalViewModel>();
                config.CreateMap<ThreeZoneCymbalViewModel, ThreeZoneCymbal>();
            }).CreateMapper();

            Drums = new ObservableCollection<DrumPadViewModel>();
            DrumsList.ItemsSource = Drums;
        }

        private DrumPadViewModel MapDrum(DrumPad drum)
        {
            switch(drum)
            {
                case EmptyDrumPad pad:
                    return _mapper.Map<EmptyDrumPadViewModel>(pad);
                case HiHatController pad:
                    return _mapper.Map<HiHatControllerViewModel>(pad);
                case ThreeZoneCymbal pad:
                    return _mapper.Map<ThreeZoneCymbalViewModel>(pad);
                case DualZoneCymbal pad:
                    return _mapper.Map<DualZoneCymbalViewModel>(pad);
                case SinglePiezoPad pad:
                    return _mapper.Map<SinglePiezoPadViewModel>(pad);
                default:
                    throw new ArgumentOutOfRangeException(nameof(drum));
            }
        }

        private DrumPad MapDrum(DrumPadViewModel drum)
        {
            switch(drum)
            {
                case EmptyDrumPadViewModel pad:
                    return _mapper.Map<EmptyDrumPad>(pad);
                case HiHatControllerViewModel pad:
                    return _mapper.Map<HiHatController>(pad);
                case ThreeZoneCymbalViewModel pad:
                    return _mapper.Map<ThreeZoneCymbal>(pad);
                case DualZoneCymbalViewModel pad:
                    return _mapper.Map<DualZoneCymbal>(pad);
                case SinglePiezoPadViewModel pad:
                    return _mapper.Map<SinglePiezoPad>(pad);
                default:
                    throw new ArgumentOutOfRangeException(nameof(drum));
            }
        }        

        private async void OnDrumModelChangedAsync(DrumPadViewModel model)
        {
            var drum = MapDrum(model);
            var result = await _connection.SetDrumParametersAsync(drum);
            if(result == null)
                throw new Exception("Error"); //todo update viewModel?
        }      

        private async void OnDrumTypeChangedAsync(DrumPadViewModel model)
        {
            var newDrum = CreateNewDrum(model);
            newDrum.ResetToDefault();
            var result = await _connection.SetDrumParametersAsync(newDrum);
            if (result == null)
                throw new Exception("Error"); //todo

            var newModel = MapDrum(result);
            newModel.OnModelChanged += OnDrumModelChangedAsync;
            newModel.OnDrumTypeChanged += OnDrumTypeChangedAsync;

            Drums.RemoveAt(model.Channel);
            Drums.Insert(model.Channel, newModel);
            DrumsList.SelectedIndex = model.Channel;
        }

        private DrumPad CreateNewDrum(DrumPadViewModel model)
        {
            switch (model.Type)
            {
                case DrumType.EmptyDrumPad:
                    return new EmptyDrumPad(model.Channel);
                case DrumType.SinglePiezoPad:
                    return new SinglePiezoPad(model.Channel);
                case DrumType.DualPiezoPad:
                    return new DualPiezoPad(model.Channel);
                case DrumType.DualZoneCymbal:
                    return new DualZoneCymbal(model.Channel);
                case DrumType.ThreeZoneCymbal:
                    return new ThreeZoneCymbal(model.Channel);
                case DrumType.HiHatController:
                    return new HiHatController(model.Channel);
                default:
                    throw new ArgumentOutOfRangeException(nameof(model));
            }
        }

        private async Task LoadConfigurationAsync()
        {
            var result = await _connection.GetAllDrumsAsync();
            if (result == null)
                throw new Exception("Can't get configuration from drum module");
            var viewModels = result.Select(MapDrum);

            Drums.Clear();
            foreach (var drum in viewModels)
            {
                Drums.Add(drum);
                drum.OnModelChanged += OnDrumModelChangedAsync;
                drum.OnDrumTypeChanged += OnDrumTypeChangedAsync;
            }

            DrumsList.SelectedIndex = 0;
        }

        private async void LoadConfigurationAsync(object sender, RoutedEventArgs e)
        {
            await LoadConfigurationAsync();
        }

        private async void ResetConfigurationAsync(object sender, RoutedEventArgs e)
        {
            var result = await _connection.ReloadSettingsAsync();
            if (!result)
                throw new Exception("Error"); //todo
            await LoadConfigurationAsync();
        }

        private async void SaveConfigurationAsync(object sender, RoutedEventArgs e)
        {
            var result = await _connection.SaveSettingsAsync();
            if (!result)
                throw new Exception("Error"); //todo
        }

        private async void RestartModuleAsync(object sender, RoutedEventArgs e)
        {
            var result = await _connection.RestartModuleAsync();
            if (!result)
                throw new Exception("Error"); //todo
            CloseDrumModuleConnection();
            await Task.Delay(3);
            await OpenDrumModuleConnectionAsync();
        }

        private async void OpenDrumModuleConnectionAsync(object sender, RoutedEventArgs e)
        {
            await OpenDrumModuleConnectionAsync();
        }

        private async Task OpenDrumModuleConnectionAsync()
        {
            if (_connection == null || !_connection.IsOpen)
            {
                _connection = new DrumModuleConnection(ComPortTxb.Text, 115200);
                _connection.OpenConnection();
                await LoadConfigurationAsync();
            }

            ((MenuItem)ComPortMenu.Items[0]).IsEnabled = false;
            ((MenuItem)ComPortMenu.Items[1]).IsEnabled = true;
        }

        private void CloseDrumModuleConnection(object sender, RoutedEventArgs e)
        {
            CloseDrumModuleConnection();
        }

        private void CloseDrumModuleConnection()
        {
            if(_connection.IsOpen)
            {
                _connection.CloseConnection();
                _connection.Dispose();
                _connection = null;
                Drums.Clear();
            }

            ((MenuItem)ComPortMenu.Items[0]).IsEnabled = true;
            ((MenuItem)ComPortMenu.Items[1]).IsEnabled = false;
        }

        public void Dispose()
        {
            _connection?.Dispose();
        }
    }

    public class DrumsTemplateSelector : DataTemplateSelector
    {
        public override DataTemplate
            SelectTemplate(object item, DependencyObject container)
        {
            if (item != null && container is FrameworkElement element && item is DrumPadViewModel drumPad)
            {
                switch (drumPad)
                {
                    case EmptyDrumPadViewModel _:
                        return element.FindResource("EmptyDrumPadTemplate") as DataTemplate;
                    case HiHatControllerViewModel _:
                        return element.FindResource("HiHatControllerTemplate") as DataTemplate;
                    case ThreeZoneCymbalViewModel _:
                        return element.FindResource("ThreeZoneCymbalTemplate") as DataTemplate;
                    case DualZoneCymbalViewModel _:
                        return element.FindResource("DualZoneCymbalTemplate") as DataTemplate;
                    case DualPiezoPadViewModel _:
                        return element.FindResource("DualPiezoPadTemplate") as DataTemplate;
                    case SinglePiezoPadViewModel _:
                        return element.FindResource("SinglePiezoPadTemplate") as DataTemplate;
                    default:
                        return null;
                        //throw new ArgumentOutOfRangeException(nameof(item)); todo
                }
            }

            return null;
        }
    }
}
