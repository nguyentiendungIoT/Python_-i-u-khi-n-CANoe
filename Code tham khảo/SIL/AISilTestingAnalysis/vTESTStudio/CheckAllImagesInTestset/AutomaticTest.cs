using System;
using System.Collections.Generic;
using System.IO;
using Vector.Tools;
using Vector.CANoe.Runtime;
using Vector.CANoe.TFS;
using SutExchange;
using SutExchange.Internal_;

using Newtonsoft.Json;

[TestClass]
public class MyTestClass
{  
  private static List<string> filePaths;
  private static CocoStructure.Dataset dataset;
  private static int[] sReceivedResultCategories;
  
  [Export]
  public static void StartUp(string directoryPath)
  {
    // Retrieve all images from the given directory path and store them in 'filePaths'
    filePaths = DataProcessing.LoadData(directoryPath);
    var testsetPath = directoryPath + @"\testset.json";
    string jsonString = File.ReadAllText(testsetPath);
    // load dataset from JSON file and store it in 'dataset'
    dataset = RetrieveDataSet(testsetPath);
    if (dataset == null)
    {
      // In case it is not possible to load the dataset, abort the test.
      Report.TestStepErrorInTestSystem("Failed to load dataset.");
      
    }
    Output.WriteLine((dataset != null) ? "Loaded dataset successfully." : "Failed to load dataset.", "Automatic Test");
  }
  
  [Export]
  public static void ProcessData()
  {
    for (int i = 0; i < filePaths.Count; i++)
    {
      Output.WriteLine($"Processing image '{filePaths[i]}'", "Automatic Test");
      ImageTC(i, filePaths[i]);
    }
      
  }
  
  [Export] [TestCase]
  public static void ImageTC(int datasetIndex, string imagePath)
  {
    // Change the test's name from 'ImageTC' to the filename
    Report.TestCaseTitle(Path.GetFileName(imagePath));
    
    // Update DO for testing
    var transmittedImage = GetTransmittedImage(imagePath);
    SutExchange.InputImage.inputImage.Value = transmittedImage;
    
    // search for filename in testset and retrieve its annotations 
    var imageName = Path.GetFileName(imagePath);
    var dsImage = dataset.Images.Find(img => img.File_name.Equals(imageName));
    if (dsImage == null)
    {
      Report.TestStepFail("Did not find the image in the ground truth file.");
      return;
    }
    
    // If we ever check for multiple result categories this will be handy
    var expectedAnnotations = dataset.Annotations.FindAll(ann => ann.Image_id == dsImage.Id);
    if (expectedAnnotations.Count == 0)
    {
      Report.TestStepFail("Did not find any matching annotations for file in testset.");
      return;
    }
    
    // wait for result
    var waitResult = Vector.CANoe.Threading.Execution.WaitForUpdate(SutExchange.ImageAnnotation.annotations, 15000 /* 15s */);
    
    // if no result is available, set ErrorInTestSystem
    if (AiTest.SutReplyAvailable.Value == 0)
    {
      Output.WriteLine("Error: SUT did not reply. Is the SUT running?");
      Report.TestStepErrorInTestSystem("SUT did not reply.");
      return;
    }
    // reset DO update flag
    AiTest.SutReplyAvailable.Value = 0;
    
    var result = new List<bool>(sReceivedResultCategories.Length);
    for (int i = 0; i < sReceivedResultCategories.Length; i++)
    {
      var category = expectedAnnotations.Find(ea => ea.category_id == sReceivedResultCategories[i]);
      if (category == null)
      {
        result.Add(false);
      }
      else
      {
        result.Add(true);
      }
    }
    
    if (result.Exists(r => r == true))
    {
      Report.TestStepPass("Found a matching category.");
      Report.AddImage(imagePath, imagePath, "50%", "50%");
    }
    else
    {
      Report.TestStepFail("Received categories did not contain any category of the ground truth.");
      Report.AddImage(imagePath, imagePath, "50%", "50%");
    }
  }
  
  public static TransmittedImage GetTransmittedImage(string imagePath)
  {
    var success = DataProcessing.ReadByteArrayFromFile(imagePath, out var targetArray, out var width, out var height);
    SutExchange.TransmittedImage image = new SutExchange.TransmittedImage();
    image.dataArray.SetBytes(targetArray);
    image.imageName = imagePath;
    
    return image;
  }
  
  [OnUpdate(SutExchange.ImageAnnotation.MemberIDs.annotations)]
  public void OnResultClassIDUpdate()
  {
    var size = SutExchange.ImageAnnotation.annotations.Value.Length;
    sReceivedResultCategories = new int[size];
    // store received result category in static variable and set DO update flag
    for (int i = 0; i < SutExchange.ImageAnnotation.annotations.Value.Length; i++)
    {
      sReceivedResultCategories[i] = (int)ImageAnnotation.annotations.Value[i].labelID.ImplValue;
    }
    AiTest.SutReplyAvailable.Value = 1;
  }
  
  public static CocoStructure.Dataset RetrieveDataSet(string pathToJson)
  {
    string jsonString = File.ReadAllText(pathToJson);
    return JsonConvert.DeserializeObject<CocoStructure.Dataset>(jsonString);
  }
}