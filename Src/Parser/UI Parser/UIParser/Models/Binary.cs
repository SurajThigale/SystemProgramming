using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UIParser.Models;

class Binary
{
    private string? binaryValue;

    public bool IsValid { get; set; }

    public Binary(int decimalValue)
    {
        binaryValue = FromDecimal(decimalValue);
        IsValid = true;
    }

    public Binary(string? binaryValue)
    {
        SetBinaryValue(binaryValue!);
    }

    public string? BinaryValue
    {
        get { return binaryValue; }
        set
        {
            SetBinaryValue(value);
        }
    }

    public int ToDecimal()
    {
        return Convert.ToInt32(binaryValue, 2);
    }

    public static string FromDecimal(int decimalValue)
    {
        string binaryString = Convert.ToString(decimalValue, 2);
        int length = binaryString.Length;

        // Adjust the length based on the binary string length
        int maxLength = Math.Max(16, length);
        return binaryString.PadLeft(maxLength, '0');
    }

    public override string? ToString()
    {
        return BinaryValue;
    }

    private void SetBinaryValue(string? value)
    {
        // Validate that the input is a valid binary string
        if (!IsBinary(value))
        {
            IsValid = false;
            binaryValue = "Invalid";
            return;
        }

        IsValid = true;
        binaryValue = value;
    }

    private static bool IsBinary(string? value)
    {
        foreach (char c in value!)
        {
            if (c != '0' && c != '1')
            {
                return false;
            }
        }
        return true;
    }
}