using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.ComponentModel;

namespace Arkserg.TeensyDrumModule.DrumSettings
{
    /// <summary>
    /// Interaction logic for NumPlusMinus.xaml
    /// </summary>
    public partial class NumericSpinner : UserControl
    {
        public event EventHandler PropertyChanged;
        public event EventHandler ValueChanged;

        public NumericSpinner()
        {
            InitializeComponent();

            txbValue.SetBinding(TextBox.TextProperty, new Binding("Value")
            {
                ElementName = "RootNumericSpinner",
                Mode = BindingMode.TwoWay,
                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
            });

            DependencyPropertyDescriptor.FromProperty(ValueProperty, typeof(NumericSpinner)).AddValueChanged(this, PropertyChanged);
            DependencyPropertyDescriptor.FromProperty(ValueProperty, typeof(NumericSpinner)).AddValueChanged(this, ValueChanged);
            DependencyPropertyDescriptor.FromProperty(DecimalsProperty, typeof(NumericSpinner)).AddValueChanged(this, PropertyChanged);
            DependencyPropertyDescriptor.FromProperty(MinValueProperty, typeof(NumericSpinner)).AddValueChanged(this, PropertyChanged);
            DependencyPropertyDescriptor.FromProperty(MaxValueProperty, typeof(NumericSpinner)).AddValueChanged(this, PropertyChanged);

            PropertyChanged += (x, y) => Validate();
        }

        /// <summary>
        /// Revalidate the object, whenever a value is changed...
        /// </summary>
        private void Validate()
        {
            // Logically, This is not needed at all... as it's handled within other properties...
            if (MinValue > MaxValue) MinValue = MaxValue;
            if (MaxValue < MinValue) MaxValue = MinValue;
            if (Value < MinValue) Value = MinValue;
            if (Value > MaxValue) Value = MaxValue;

            Value = decimal.Round(Value, Decimals);
        }

        private void CmdUpClick(object sender, RoutedEventArgs e) => Value += Step;

        private void CmdDownClick(object sender, RoutedEventArgs e) => Value -= Step;

        private void TxbMainLoaded(object sender, RoutedEventArgs e) 
            => ValueChanged?.Invoke(this, new EventArgs());

        #region ValueProperty

        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            "Value",
            typeof(decimal),
            typeof(NumericSpinner),
            new PropertyMetadata(new decimal(0)));

        public decimal Value
        {
            get => (decimal)GetValue(ValueProperty);
            set
            {
                if (value < MinValue)
                    value = MinValue;
                if (value > MaxValue)
                    value = MaxValue;
                SetValue(ValueProperty, value);
                ValueChanged?.Invoke(this, new EventArgs());
            }
        }


        #endregion

        #region StepProperty

        public static readonly DependencyProperty StepProperty = DependencyProperty.Register(
            "Step",
            typeof(decimal),
            typeof(NumericSpinner),
            new PropertyMetadata(new decimal(0.1)));

        public decimal Step
        {
            get => (decimal)GetValue(StepProperty);
            set => SetValue(StepProperty, value);
        }

        #endregion

        #region DecimalsProperty

        public static readonly DependencyProperty DecimalsProperty = DependencyProperty.Register(
            "Decimals",
            typeof(int),
            typeof(NumericSpinner),
            new PropertyMetadata(2));

        public int Decimals
        {
            get => (int)GetValue(DecimalsProperty);
            set => SetValue(DecimalsProperty, value);
        }

        #endregion

        #region MinValueProperty

        public static readonly DependencyProperty MinValueProperty = DependencyProperty.Register(
            "MinValue",
            typeof(decimal),
            typeof(NumericSpinner),
            new PropertyMetadata(decimal.MinValue));

        public decimal MinValue
        {
            get => (decimal)GetValue(MinValueProperty);
            set
            {
                if (value > MaxValue)
                    MaxValue = value;
                SetValue(MinValueProperty, value);
            }
        }

        #endregion

        #region MaxValueProperty

        public static readonly DependencyProperty MaxValueProperty = DependencyProperty.Register(
            "MaxValue",
            typeof(decimal),
            typeof(NumericSpinner),
            new PropertyMetadata(decimal.MaxValue));

        public decimal MaxValue
        {
            get => (decimal)GetValue(MaxValueProperty);
            set
            {
                if (value < MinValue)
                    value = MinValue;
                SetValue(MaxValueProperty, value);
            }
        }

        #endregion
    }
}
