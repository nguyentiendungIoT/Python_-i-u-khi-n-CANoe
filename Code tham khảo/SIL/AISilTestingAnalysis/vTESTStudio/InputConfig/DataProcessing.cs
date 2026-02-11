using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Web;

using Vector.Tools;

public static class DataProcessing
{
  public static List<string> LoadData(string directoryPath)
  {
    List<string> configDo = null;
    if (string.IsNullOrEmpty(directoryPath))
    {
      return configDo;
    }

    try
    {
      var filesInPath = Directory.GetFiles(directoryPath);
      configDo = new List<string>(filesInPath.Length);
      if (filesInPath.Length == 0)
      {
        // Directory is empty -> skip further actions
        Output.WriteLine("Warning: The provided directory is empty. No analysis will be performed.");
        return configDo;
      }
      // Upper bound for list length - could be optimized
      foreach (var filePath in filesInPath)
      {
        if (!IsImage(filePath))
        {
          continue;
        }
        configDo.Add(filePath);
      }
      Output.WriteLine($"Found {filesInPath.Length} files in path ({configDo.Count} images).");
    }
    catch (ArgumentNullException)
    {
      Output.WriteLine($"An error occurred while reading files from the path. The provided path was null.");
    }
    catch (PathTooLongException)
    {
      Output.WriteLine($"An error occurred while reading files from the path ({directoryPath}). The provided path was too long.");
    }
    catch (DirectoryNotFoundException)
    {
      Output.WriteLine($"An error occurred while reading files from the path ({directoryPath}). The provided path could not be found.");
    }
    catch (Exception e)
    {
      Output.WriteLine($"An error occurred while reading files from the path ({directoryPath}).\nError message:");
      Output.WriteLine(e.ToString());
      throw;
    }
    return configDo;
  }

  public static bool IsImage(string filePath)
  {
    var mime = MimeMapping.GetMimeMapping(filePath);
    return mime.StartsWith("image/");
  }

  public static bool ReadByteArrayFromFile(string filePath, out byte[] outputArray, out uint? width, out uint? height)
  {
    filePath = Path.GetFullPath(filePath);
    try
    {
      Image img = Image.FromFile(filePath);
      width = (uint)img.Width;
      height = (uint)img.Height;
      using (MemoryStream ms = new MemoryStream())
      {
        img.Save(ms, System.Drawing.Imaging.ImageFormat.Png);
        outputArray = ms.ToArray();
      }
      return true;
    }
    catch (Exception e)
    {
      Output.WriteLine("C#: There was an error while reading an image file: " + e.Message);
      Output.WriteLine(e.ToString());
      outputArray = null;
      width = null;
      height = null;
      return false;
    }
  }
}