using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CocoStructure
{
  public class Info
  {
    public string Description { get; set; }
    public string Url { get; set; }
    public string Version { get; set; }
    public int Year { get; set; }
    public string Contributor { get; set; }
    public string Date_created { get; set; }
  }

  public class License
  {
    public string Url { get; set; }
    public int Id { get; set; }
    public string Name { get; set; }
  }

  public class Image
  {
    public int License { get; set; }
    public string File_name { get; set; }
    public string Coco_url { get; set; }
    public int Height { get; set; }
    public int Width { get; set; }
    public string Date_captured { get; set; }
    public string Flickr_url { get; set; }
    public int Id { get; set; }
  }

  public class Annotation
  {
    public IList<IList<double>> Segmentation { get; set; }
    public double Area { get; set; }
    public int IsCrowd { get; set; }
    public int Image_id { get; set; }
    public IList<double> bbox { get; set; }
    public int category_id { get; set; }
    public int id { get; set; }
  }

  public class Category
  {
    public string SuperCategory { get; set; }
    public int Id { get; set; }
    public string Name { get; set; }
  }

  public class Dataset
  {
    public Info Info { get; set; }
    public IList<License> Licenses { get; set; }
    public List<Image> Images { get; set; }
    public List<Annotation> Annotations { get; set; }
    public List<Category> Categories { get; set; }
  }
}
