/// <summary>
/// 		word	 交易所代码
/// </summary>
public class wMarketID : BaseReq
{
	/// <summary>
	///   商品类型
	/// </summary>
	public short wType;
	/// <summary>
	///  　排序方式个数
	/// </summary>
	public short wSortCount;
	/// <summary>
	///   排序方式组
	/// </summary>
	public SortDatas>[] bSortDatas;
	/// <summary>
	///   开始序号
	/// </summary>
	public short wFrom;
	/// <summary>
	///  每组最大返回的股票数据量（目前是平均分配模式，每组都相同）
	/// </summary>
	public short wCount;
	/// <summary>
	///  
	/// </summary>
	public  ;
	public override void PackToBytes(BinaryWriter writer)
	{
		writer.write(wType);
		writer.write(wSortCount);
		
		writer.write(wFrom);
		writer.write(wCount);
		can't find the type of 
	}

	public wMarketID() 
	{
	}
}
