using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;

using Vector.Tools;
using Vector.CANoe.Runtime;

using SutExchange.Internal_;
using _GraphicalElements.DataModel;

public class PanelHelper_DataMediator : MeasurementScript
{
  #region members

  private static uint mContainerSize = 2500;
  private static readonly string sContainerName = "DefaultContainer";
  private IGraphicalElementContainer mContainer = null;
  private static readonly string sInputImageName = "InputImage";
  private IImageElement mInputImageElement = null;
  private static readonly string sOutputImageName = "SaliencyMap";
  private IImageElement mOutputImageElement = null;

  private static readonly string sBoundingBox1Name = "BoundingBox1";
  private static readonly string sBoundingBox2Name = "BoundingBox2";
  private static readonly string sBoundingBox3Name = "BoundingBox3";
  private Dictionary<string, IRectangleElement> mStringToRectangleDict = new Dictionary<string, IRectangleElement>();

  #endregion

  #region Panel Helper methods

  [OnChange(typeof(Analysis.Sysvar_StartTransmission))]
  public void StartTransmissionMethod()
  {
    // Only trigger on a change to 0 (onRelease)
    if (Analysis.Sysvar_StartTransmission.Value == 0)
    {
      return;
    }

    // Activate updates of AI visualizer
    PanelControl.Sysvar_TestMode.Value = 1;
    TransmitImage();
    Output.WriteLine("Sent image to SUT.");
  }
  private void TransmitImage()
  {
    ResetImage(mOutputImageElement);
    var filePath = Analysis.Sysvar_FileBasePath.Value + Analysis.Sysvar_Filename.Value;
    var success = ReadByteArrayFromFile(filePath, out var targetArray, out var width, out var height);
    if (success)
    {
      InputImage.inputImage.imageName = Analysis.Sysvar_Filename.Value;
      InputImage.inputImage.width = width.Value;
      InputImage.inputImage.height = height.Value;
      if (InputImage.IsConnected())
      {
        InputImage.inputImage.dataArray = targetArray;
      }
      else
      {
        Output.WriteLine("C#: InputImageSender was not connected!");
      }
    }
  }

  [OnChange(typeof(PanelControl.Sysvar_GitRepo))]
  public void GitHubRepoOpen()
  {
    Process.Start("https://github.com/vectorgrp/AI-SUT-Demo");
  }
  #endregion

  #region AI Visualizer helper methods

  public override void Start()
  {
    InitializeAiVisualizer();
      
  }

  private void InitializeAiVisualizer()
  {
    if (mContainer != null)
    {
      ResetBoundingBoxes();
      return;
    }
    else
    {
      Output.WriteLine("Initializing AI visualizer...");
    }

    // container
    mContainer = GetOrCreateContainer(sContainerName);
    if (mContainer != null)
    {
      mContainer.container_data.position.x.ImplValue = 0;
      mContainer.container_data.position.y.ImplValue = 0;
      mContainer.container_data.dimension.height.ImplValue = mContainerSize;
      mContainer.container_data.dimension.width.ImplValue = mContainerSize;
      // Create new element and add it to a container
      mInputImageElement = GetOrCreateElement(sInputImageName, sContainerName, EElementType.Image) as IImageElement;
      mOutputImageElement = GetOrCreateElement(sOutputImageName, sContainerName, EElementType.Image) as IImageElement;
      ResetImage(mInputImageElement);
      ResetImage(mOutputImageElement);

      InitializeBoundingBox(sBoundingBox1Name);
      InitializeBoundingBox(sBoundingBox2Name);
      InitializeBoundingBox(sBoundingBox3Name);
    }
    else
    {
      Output.WriteLine("Unable to create container for AI visualizer.");
      throw new InvalidOperationException("Unable to create container for AI visualizer.");
    }
  }

  private void InitializeBoundingBox(string bbName)
  {
    var boundingBox = GetOrCreateElement(bbName, sContainerName, EElementType.Rectangle) as IRectangleElement;
    if (boundingBox == null)
    {
      Output.WriteLine("Unable to initialize bounding box.");
      throw new InvalidOperationException("Unable to initialize bounding box.");
    }
    mStringToRectangleDict.Add(bbName, boundingBox);
    ResetRectangle(bbName);
  }

  private void UpdateBoundingBox(string identifier, Rectangle_ReadOnly rectangle, int index)
  {
    var result = mStringToRectangleDict.TryGetValue(identifier, out var boundingBox);
    if (!result)
    {
      Output.WriteLine($"Warning: Tried to retrieve invalid bounding box '{identifier}'.");
      return;
    }
    boundingBox.shape_data.stroke_size = 2;

    boundingBox.element_data.position.x.ImplValue = rectangle.xMin.ImplValue;
    boundingBox.element_data.position.y.ImplValue = rectangle.yMin.ImplValue;

    boundingBox.element_data.dimension.width.ImplValue = rectangle.xMax.ImplValue - rectangle.xMin.ImplValue;
    boundingBox.element_data.dimension.height.ImplValue = rectangle.yMax.ImplValue - rectangle.yMin.ImplValue;

    boundingBox.shape_data.text_data.content = AiVisualizer.rectangle.Text.Content.Value;
    boundingBox.shape_data.text_data.font_size.ImplValue = (uint)AiVisualizer.rectangle.Text.FontSize.Value;

    if (index < 3)
    {
      boundingBox.shape_data.stroke_color.R.ImplValue = 191;
      boundingBox.shape_data.stroke_color.G.ImplValue = 32;
      boundingBox.shape_data.stroke_color.B.ImplValue = 75;
      boundingBox.shape_data.stroke_color.A.ImplValue = 255;
    }
  }

  private void ResetBoundingBoxes()
  {
    foreach (var key in mStringToRectangleDict.Keys)
    {
      ResetRectangle(key);
    }
  }
  private void ResetRectangle(string rectName)
  {
    var success = mStringToRectangleDict.TryGetValue(rectName, out var boundingBox);
    if (!success)
    {
      Output.WriteLine($"Warning: Unable to retrieve bounding box '{rectName}'");
      return;
    }

    boundingBox.shape_data.fill_color.R.ImplValue = 255;
    boundingBox.shape_data.fill_color.G.ImplValue = 255;
    boundingBox.shape_data.fill_color.B.ImplValue = 255;
    boundingBox.shape_data.fill_color.A.ImplValue = 0;

    boundingBox.shape_data.stroke_color.R.ImplValue = 0;
    boundingBox.shape_data.stroke_color.G.ImplValue = 0;
    boundingBox.shape_data.stroke_color.B.ImplValue = 0;
    boundingBox.shape_data.stroke_color.A.ImplValue = 0;
    boundingBox.shape_data.stroke_size = 0;

    boundingBox.element_data.position.x.ImplValue = 5;
    boundingBox.element_data.position.y.ImplValue = 5;
    boundingBox.element_data.dimension.width.ImplValue = 2;
    boundingBox.element_data.dimension.height.ImplValue = 2;
  }
  #endregion

  #region SUT response methods

  [OnUpdate(ImageAnnotation.MemberIDs.annotations)]
  public void BoundingBoxTransfer()
  {
    Output.WriteLine("Received response from SUT.");
    // Do not update while automatic test is running
    if (PanelControl.Sysvar_TestMode.Value == 0)
    {
      return;
    }

    InitializeAiVisualizer();
    UpdateBoundingBox(sBoundingBox1Name, ImageAnnotation.annotations.Value[0].boundingBox, 0);
    UpdateBoundingBox(sBoundingBox2Name, ImageAnnotation.annotations.Value[1].boundingBox, 1);
    UpdateBoundingBox(sBoundingBox3Name, ImageAnnotation.annotations.Value[2].boundingBox, 2);
  }

  [OnUpdate(InputImage.MemberIDs.inputImage)]
  public void InputImageReceived()
  {
    // Do not update while automatic test is running
    if (PanelControl.Sysvar_TestMode.Value == 0)
    {
      return;
    }

    InitializeAiVisualizer();

    byte[] targetArray;
    var inputImage = InputImage.inputImage;

    if (inputImage.dataArray != null && inputImage.dataArray.Length > 0)
    {
      // read byte array
      targetArray = inputImage.dataArray;
    }
    else
    {
      Output.WriteLine("Warning: Received input image that contained no data. Canceling remaining operation.");
      return;
    }

    UpdateImage(mInputImageElement, targetArray, inputImage.height, inputImage.width);
  }

  [OnUpdate(ImageAnnotation.MemberIDs.outputImage)]
  public void OutputImageReceived()
  {
    // Do not update while automatic test is running
    if (PanelControl.Sysvar_TestMode.Value == 0)
    {
      return;
    }

    InitializeAiVisualizer();

    byte[] targetArray;
    var outputImage = ImageAnnotation.outputImage;

    if (outputImage.dataArray != null && outputImage.dataArray.Length > 0)
    {
      // read byte array
      targetArray = outputImage.dataArray;
    }
    else
    {
      Output.WriteLine("Warning: Received output image that contained no data. Canceling remaining operation.");
      return;
    }

    UpdateImage(mOutputImageElement, targetArray, outputImage.height, outputImage.width);
  }

  void UpdateImage(IImageElement imageElement, byte[] targetArray, uint height, uint width)
  {
    mContainer.container_data.dimension.height.ImplValue = height;
    mContainer.container_data.dimension.width.ImplValue = width;

    imageElement.element_data.position.x.ImplValue = 0;
    imageElement.element_data.position.y.ImplValue = 0;

    imageElement.element_data.dimension.height.ImplValue = height;
    imageElement.element_data.dimension.width.ImplValue = width;

    imageElement.image_data.raw_data = targetArray;
  }

  void ResetImage(IImageElement imageElement)
  {
    imageElement.element_data.position.x.ImplValue = 0;
    imageElement.element_data.position.y.ImplValue = 0;

    imageElement.element_data.dimension.height.ImplValue = 1;
    imageElement.element_data.dimension.width.ImplValue = 1;

    var img = new Bitmap(1, 1);
    img.SetPixel(0, 0, Color.FromArgb(0, 255, 0, 0));

    imageElement.image_data.raw_data = (byte[])new ImageConverter().ConvertTo(img, typeof(byte[])); ;
  }
  #endregion

  #region Auxiliary methods

  // This is a temporary auxiliary method that will be removed in a future version of this demo
  IGraphicalElement GetOrCreateElement(string name, string containerName, EElementType elementType)
  {
    var result = GraphicalElements.GeFactory.GetOrCreateElement.Call(name, containerName, (UInt32)elementType);
    if (!String.IsNullOrEmpty(result))
    {
      return DORegistry.LookupDistributedObject<IGraphicalElement>(name, "GraphicalElements");
    }
    return null;
  }

  IGraphicalElementContainer GetOrCreateContainer(string containerName)
  {
    var result = GraphicalElements.GeFactory.GetOrCreateElementContainer.Call(sContainerName);
    if (!String.IsNullOrEmpty(result))
    {
      return DORegistry.LookupDistributedObject<IGraphicalElementContainer>(sContainerName, "GraphicalElements");
    }
    else
    {
      return null;
    }
  }

  public static bool ReadByteArrayFromFile(string filePath, out byte[] outputArray, out uint? width, out uint? height)
  {
    filePath = Path.GetFullPath(filePath);
    try
    {
      var img = System.Drawing.Image.FromFile(filePath);
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
      Output.WriteLine("There was an error while loading the image file:");
      Output.WriteLine(e.ToString());
      outputArray = null;
      width = null;
      height = null;
      return false;
    }
  }
  #endregion
}