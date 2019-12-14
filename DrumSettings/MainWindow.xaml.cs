using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using Arkserg.TeensyDrumModule.DrumSettings.Models;

namespace Arkserg.TeensyDrumModule.DrumSettings
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ObservableCollection<DrumPadViewModel> Drums { get; set; }

        public MainWindow()
        {
            InitializeComponent();

            Drums = new ObservableCollection<DrumPadViewModel>
            {
                //new SinglePiezoPadViewModel {Channel = 0, Name = "Snare"},
                //new DualZoneCymbalViewModel {Channel = 1, Name = "Crash"},
                new ThreeZoneCymbalViewModel {Channel = 2, Name = "Ride"},
                //new HiHatControllerViewModel {Channel = 3, Name = "HiHat Controller"},
                new EmptyDrumPadViewModel {Channel = 4},
                new EmptyDrumPadViewModel {Channel = 5},
                new EmptyDrumPadViewModel {Channel = 6},
                new EmptyDrumPadViewModel {Channel = 7},
                new EmptyDrumPadViewModel {Channel = 8},
                new EmptyDrumPadViewModel {Channel = 9},
                new EmptyDrumPadViewModel {Channel = 10},
                new EmptyDrumPadViewModel {Channel = 11},                
            };

            foreach (var drum in Drums)
                drum.OnModelChanged += OnDrumModelChanged;

            //DrumsList.ItemsSource = Drums;
        }

        private void OnDrumModelChanged(DrumPadViewModel model)
        {

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
                    case ThreeZoneCymbalViewModel p:
                        return element.FindResource("ThreeZoneCymbalTemplate") as DataTemplate;
                    case SinglePiezoPadViewModel p:
                        return element.FindResource("SinglePiezoPadTemplate") as DataTemplate;
                    default:
                        return null;
                }
            }

            return null;
        }
    }
}
