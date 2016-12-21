<?php
/**
 * Enter class description
 */
class OrdersListCMS
{
	private $m_list;
	
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		$sql = "SELECT * FROM `orders` where `orderType` = ".intval($_GET["orderType"])." order by `added_date` desc";
		$this->m_list = DB::QueryToArray($sql);
	}
	
	function DoAction()
	{
		// do actions for Pr_type list
		switch ( DB::POST("order_action") )
		{
			case 'change_email_1' :
				if ( ($email = DB::POST('email')) )
				{
					$query = "update `ordes_email` set `email` = '$email' where `orderType` = 1";
					DB::Query($query);
				}
				break;
			case 'change_email_2' :
				if ( ($email = DB::POST('email')) )
				{
					$query = "update `ordes_email` set `email` = '$email' where `orderType` = 2";
					DB::Query($query);
				}
				break;
			case 'delete_orders':
				$orderIDs = DB::POST("orderID");
				if ( is_array($orderIDs) && count($orderIDs)>0 )
				{
					$orderIDs = implode(", ", $orderIDs);
					$query = "delete from `orders` where `ID` in ($orderIDs)";
					DB::Query($query);
				}
				
				$sql = "SELECT * FROM `orders` where `orderType` = ".intval($_GET["orderType"])." order by `added_date` desc";
				$this->m_list = DB::QueryToArray($sql);
				break;
		}
	}

	/**
	* Display menu
	*
	*/
	function DisplayMenu()
	{
		?>
		<div class="someWTitleBox">Orders</div>
		<div class="someWBox">
			<div class="directoryMenu1">
				<?php
				if ( isset($_GET["orderType"]) && $_GET["orderType"]==1 )
				{
					?>
					Заказ мероприятия
					<?php
				}
				else 
				{
					?>
					<a href="?orderType=1">Заказ мероприятия</a>
					<?php
				}
				?>
			</div>
			<div class="directoryMenu1">
				<?php
				if ( isset($_GET["orderType"]) && $_GET["orderType"]==2 )
				{
					?>
					Заказ билетов
					<?php
				}
				else 
				{
					?>
					<a href="?orderType=2">Заказ билетов</a>
					<?php
				}
				?>
			</div>
		</div>
		<?php
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		if ( isset($_GET["orderType"]) && $_GET["orderType"]==1 )
		{
			$query = "select `email` from `ordes_email` where `orderType` = 1";
			$email = DB::QueryToValue($query);
			?>
			<script src="http://code.jquery.com/jquery-latest.min.js" type="text/javascript"></script>
			<link rel="stylesheet" href="js/slimtable/slimtable.css">
			<script src="js/slimtable/slimtable.js"></script>
			<script src="js/jquery.table2excel.js"></script>
			
			<form action="" method="POST">
				<input type="hidden" name="order_action" value="change_email_1">
				<table cellspacing="0" cellpadding="0" border="0" width="100%" style="margin-bottom:20px;">
					<tr>
						<td width="50">Email</td>
						<td width="300"><input type="text" name="email" value="<?php echo $email;?>" style="width:450px;height:25px;"></td>
						<td><input type="submit" value="Done"></td>
					</tr>
				</table>
			</form>
			
			<form action="" method="POST" onsubmit="return confirm('Are you sure?');">
				<input type="hidden" name="order_action" value="delete_orders">
				<table cellspacing="0" cellpadding="0" border="0" width="100%" style="margin-bottom:20px;">
					<tr>
						<td align="left"><input type="submit" value="Export" id="orders-export"></td>
						<td align="right"><input type="submit" value="Delete"></td>
					</tr>
				</table>
				<table id="orders-table" class="bookingTableArea" cellspacing="0" cellpadding="0" border="0" width="100%" style="border-left: 1px solid #777777;border-top: 1px solid #777777;">
					<thead>
						<tr class="bookinItemS">
							<th align="center">#</th>
							<th nowrap>Тип мероприятия</th>
							<th>Город</th>
							<th nowrap>Планируемая дата</th>
							<th>Имя</th>
							<th>Телефон</th>
							<th nowrap>Адрес эл. почты</th>
							<th>Сообщение</th>
							<th nowrap>Дата заказа</th>
						</tr>
					</thead>
					<tbody>
						<?php
						$list = $this->m_list;
						for ( $i=0; $i<count($list); $i++ )
						{
							?>
							<tr class="tableUneven">
								<td><input type="checkbox" name="orderID[]" value="<?php echo $list[$i]["ID"];?>"></td>
								<td><?php echo $list[$i]["type"];?></td>
								<td><?php echo $list[$i]["city"];?></td>
								<td><?php echo $list[$i]["date"];?></td>
								<td><?php echo $list[$i]["name"];?></td>
								<td><?php echo $list[$i]["phone"];?></td>
								<td><?php echo $list[$i]["email"];?></td>
								<td><?php echo $list[$i]["message"];?></td>
								<td><?php echo $list[$i]["added_date"];?></td>
							</tr>
							<?php
						}
						?>
					</tbody>
				</table>
			</form>
			
			<table id="orders-table-export" width="100%" style="display:none;">
				<thead>
					<tr>
						<th nowrap>Тип мероприятия</th>
						<th>Город</th>
						<th nowrap>Планируемая дата</th>
						<th>Имя</th>
						<th>Телефон</th>
						<th nowrap>Адрес эл. почты</th>
						<th>Сообщение</th>
						<th nowrap>Дата заказа</th>
					</tr>
				</thead>
				<tbody>
					<?php
					$list = $this->m_list;
					for ( $i=0; $i<count($list); $i++ )
					{
						?>
						<tr>
							<td><?php echo $list[$i]["type"];?></td>
							<td><?php echo $list[$i]["city"];?></td>
							<td><?php echo $list[$i]["date"];?></td>
							<td><?php echo $list[$i]["name"];?></td>
							<td><?php echo $list[$i]["phone"];?></td>
							<td><?php echo $list[$i]["email"];?></td>
							<td><?php echo $list[$i]["message"];?></td>
							<td><?php echo $list[$i]["added_date"];?></td>
						</tr>
						<?php
					}
					?>
				</tbody>
			</table>
			
			<script type='text/javascript'>
			$("#orders-table").slimtable({
				colSettings: [ 
					{ colNumber: 0, enableSort: false }
				]
			});

			$("#orders-export").click(function(e){
				e.preventDefault();
				$("#orders-table-export").table2excel({
					exclude: ".noExl",
					name: "Excel Document Name",
					filename: "Orders",
					fileext: ".xls",
					exclude_img: true,
					exclude_links: true,
					exclude_inputs: true
				});
			});
			</script>
			<?php
		}
		elseif ( isset($_GET["orderType"]) && $_GET["orderType"]==2 )
		{
			$query = "select `email` from `ordes_email` where `orderType` = 2";
			$email = DB::QueryToValue($query);
			?>
			<script src="http://code.jquery.com/jquery-latest.min.js" type="text/javascript"></script>
			<link rel="stylesheet" href="js/slimtable/slimtable.css">
			<script src="js/slimtable/slimtable.js"></script>
			<script src="js/jquery.table2excel.js"></script>
			
			<form action="" method="POST">
				<input type="hidden" name="order_action" value="change_email_2">
				<table cellspacing="0" cellpadding="0" border="0" width="100%" style="margin-bottom:20px;">
					<tr>
						<td width="50">Email</td>
						<td width="300"><input type="text" name="email" value="<?php echo $email;?>" style="width:450px;height:25px;"></td>
						<td><input type="submit" value="Done"></td>
					</tr>
				</table>
			</form>
			
			<form action="" method="POST" onsubmit="return confirm('Are you sure?');">
				<input type="hidden" name="order_action" value="delete_orders">
				<table cellspacing="0" cellpadding="0" border="0" width="100%" style="margin-bottom:20px;">
					<tr>
						<td align="left"><input type="submit" value="Export" id="orders-export"></td>
						<td align="right"><input type="submit" value="Delete"></td>
					</tr>
				</table>
				<table id="orders-table" class="bookingTableArea" cellspacing="0" cellpadding="0" border="0" width="100%" style="border-left: 1px solid #777777;border-top: 1px solid #777777;">
					<thead>
						<tr class="bookinItemS">
							<th align="center">#</th>
							<th nowrap>Место и время</th>
							<th nowrap>Имя, Фамилия</th>
							<th nowrap>Эл. почта</th>
							<th>Телефон</th>
							<th>Сообщение</th>
							<th nowrap>Дата заказа</th>
						</tr>
					</thead>
					<tbody>
						<?php
						$list = $this->m_list;
						for ( $i=0; $i<count($list); $i++ )
						{
							?>
							<tr class="tableUneven">
								<td><input type="checkbox" name="orderID[]" value="<?php echo $list[$i]["ID"];?>"></td>
								<td><?php echo $list[$i]["date_place"];?></td>
								<td><?php echo $list[$i]["name"];?></td>
								<td><?php echo $list[$i]["email"];?></td>
								<td><?php echo $list[$i]["phone"];?></td>
								<td><?php echo $list[$i]["message"];?></td>
								<td><?php echo $list[$i]["added_date"];?></td>
							</tr>
							<?php
						}
						?>
					</tbody>
				</table>
			</form>
			
			<table id="orders-table-export" width="100%" style="display:none;">
				<thead>
					<tr>
						<th nowrap>Место и время</th>
						<th nowrap>Имя, Фамилия</th>
						<th nowrap>Эл. почта</th>
						<th>Телефон</th>
						<th>Сообщение</th>
						<th nowrap>Дата заказа</th>
					</tr>
				</thead>
				<tbody>
					<?php
					$list = $this->m_list;
					for ( $i=0; $i<count($list); $i++ )
					{
						?>
						<tr>
							<td><?php echo $list[$i]["date_place"];?></td>
							<td><?php echo $list[$i]["name"];?></td>
							<td><?php echo $list[$i]["email"];?></td>
							<td><?php echo $list[$i]["phone"];?></td>
							<td><?php echo $list[$i]["message"];?></td>
							<td><?php echo $list[$i]["added_date"];?></td>
						</tr>
						<?php
					}
					?>
				</tbody>
			</table>
			
			<script type='text/javascript'>
			$("#orders-table").slimtable({
				colSettings: [ 
					{ colNumber: 0, enableSort: false }
				]
			});

			$("#orders-export").click(function(e){
				e.preventDefault();
				$("#orders-table-export").table2excel({
					exclude: ".noExl",
					name: "Excel Document Name",
					filename: "Orders",
					fileext: ".xls",
					exclude_img: true,
					exclude_links: true,
					exclude_inputs: true
				});
			});
			</script>
			<?php
		}
	}
}
?>