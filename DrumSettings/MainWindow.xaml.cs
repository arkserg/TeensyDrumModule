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
                new SinglePiezoPadViewModel {Id = 0, Name = "Snare"},
                new SinglePiezoPadViewModel {Id = 1, Name = "High Tom 1"},
                new SinglePiezoPadViewModel {Id = 2, Name = "High Tom 2"},
                new SinglePiezoPadViewModel {Id = 3, Name = "Floor Tom 1"},
                new SinglePiezoPadViewModel {Id = 4, Name = "Floor Tom 2"},
                new SinglePiezoPadViewModel {Id = 5, Name = "Bass"},
                new SinglePiezoPadViewModel {Id = 6, Name = "CowBell"},
                new YamahaPcyCymbalViewModel {Id = 7, Name = "HiHat"},
                new YamahaPcyCymbalViewModel {Id = 8, Name = "Crash1"},
                new YamahaPcyCymbalViewModel {Id = 9, Name = "Ride"},
                new YamahaPcyCymbalViewModel {Id = 10, Name = "Crash2"},
                new YamahaPcyCymbalViewModel {Id = 11, Name = "China"},
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
                    case YamahaPcyCymbalViewModel p:
                        return element.FindResource("YamahaPcyTemplate") as DataTemplate;
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
