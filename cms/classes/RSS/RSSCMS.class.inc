<?php
// includes
Site::IncludeFile ( 'classes/RSS/RSS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_type/Pr_typeList.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuList.class.inc' );
Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );

class RSSCMS extends CMSObject implements CMSInterface
{
	protected $m_fileName;
	
	function __construct($fileName = null)
	{
		parent::__construct ();
		
		$this->m_fileName = $fileName;
		
		$this->ReadNameStrings('classes/RSS/langs/', 'rss.xml');
	}
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		$rss = new RSS ( 'rss/' . $this->m_fileName );
		$res = $rss->Parse ();
		
		if ($res)
		{
			// read name string /////////////////////////
			$doneS = $this->GetNameString('done');
			$deleteS = $this->GetNameString('delete');
			$optionsS = $this->GetNameString('options');
			$channelS = $this->GetNameString('channel');
			$newItemS = $this->GetNameString('newItem');
			$itemsS = $this->GetNameString('items');
			
			// channel names
			$channelTitleS = $this->GetNameString('channelTitle');
			$channelLinkS = $this->GetNameString('channelLink');
			$channelDescriptionS = $this->GetNameString('channelDescription');
			$channelLanguageS = $this->GetNameString('channelLanguage');
			$channelCopyrightS = $this->GetNameString('channelCopyright');
			$channelLastBuildDateS = $this->GetNameString('channelLastBuildDate');
			$channelCategoryS = $this->GetNameString('channelCategory');
			$channelGeneratorS = $this->GetNameString('channelGenerator');
			$channelTTLS = $this->GetNameString('channelTTL');
			$channelImageTitleS = $this->GetNameString('channelImageTitle');
			$channelImageLinkS = $this->GetNameString('channelImageLink');
			$channelImageURLS = $this->GetNameString('channelImageURL');
			$channelWidthS = $this->GetNameString('channelWidth');
			$channelHeightS = $this->GetNameString('channelHeight');

			/////////////////////////////////////////
			$actionName = $this->GetFormActionName();
			
			// common options
			$title = $rss->GetTitle ();
			$link = $rss->GetLink();
			$copyright = $rss->GetCopyright ();
			$category = $rss->GetCategory ();
			$description = $rss->GetDescription ();
			$language = $rss->GetLanguage ();
			$lastBuildDate = $rss->GetLastBuildDate ();
			$ttl = $rss->GetTtl ();
			
			// image optoins
			$imageTitle = $rss->GetImageTitle();
			$imageLink = $rss->GetImageLink();
			$imageURL = $rss->GetImageURL();
			$imageWidth = $rss->GetImageWidth();
			$imageHeight= $rss->GetImageHeight();
			
			// overloading properties
			$linkName = 'RSSListID';
			$currentID = $this->m_fileName;
			
			$form = null;
		
			if (true)
			{
				$form = "<div  class='titleColR'>
						 <form method='post' class='clearPD'> 
						 <input type='hidden' name='$actionName' value='delete' />
						 <input type='hidden' name='RSSListID' value='{$this->m_fileName}' />
						 <input type='submit' onclick='return confirm(\"Are You Sure?\");' value='$deleteS' />
						 </form>
						 </div>";
			}
			
			$this->DisplayTitle($this->m_fileName, $form);

			print('<br />');

			$this->DisplayFormHeadr('publish');
        	$this->DisplayHidden($linkName, $currentID); 
        ?>
        <table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
          <tr>
            <td>
                <div class="titleColL"><h1>Last Build Date: <?php echo $lastBuildDate?></h1></div>
                <div class="titleColR"><input type="submit" value="Update" /></div>
                <br class="clearFloat" />
        	</td>
          </tr>
        </table>
        </form>
        <div class="someGTitleBox"><?php echo $channelS?></div>
        <div class="someGBox">
			<div id="CollapsiblePanel1" class="CollapsiblePanel">
                <div class="CollapsiblePanelTab" tabindex="0"><?php echo $optionsS?></div>
                <div class="CollapsiblePanelContent">
                    <div class="CollapsiblePanelContentPD">
                        <?php
                        	$this->DisplayFormHeadr ( 'changeInfo' ); 
                        	$this->DisplayHidden($linkName, $currentID);
                        ?>
                       <div class="userFormLabel"><label><strong><?php echo $channelTitleS?></strong></label></div>
                       <div style="float:left"><input name="title" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $title?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><strong><?php echo $channelLinkS?></strong></label></div>
                       <div style="float:left"><input name="link" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $link?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><strong><?php echo $channelDescriptionS?></strong></label></div>
                       <div style="float:left"><input name="description" type="text" size="64" maxlength="256" style="font-size:14px" value="<?php echo $description?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelLanguageS?></label></div>
                       <div style="float:left"><input name="language" type="text" size="16" maxlength="16" style="font-size:14px" value="<?php echo $language?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelCopyrightS?></label></div>
                       <div style="float:left"><input name="copyright" type="text" size="36" maxlength="256" style="font-size:14px" value="<?php echo $copyright?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelLastBuildDateS?></label></div>
                       <div style="float:left"><input name="lastBuildDate" type="text" size="36" maxlength="128" style="font-size:14px" value="<?php echo $lastBuildDate?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelCategoryS?></label></div>
                       <div style="float:left"><input name="category" type="text" size="36" maxlength="256" style="font-size:14px" value="<?php echo $category?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelGeneratorS?></label></div>
                       <div style="float:left"><input name="" type="text" size="36" maxlength="128" readonly="readonly" value="Siteius CMS Feed Generator 0.1" style="font-size:14px" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelTTLS?></label></div>
                       <div style="float:left"><input name="ttl" type="text" size="16" maxlength="32" style="font-size:14px" value="<?php echo $ttl?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelImageTitleS?></label></div>
                       <div style="float:left"><input name="imageTitle" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $imageTitle?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelImageLinkS?></label></div>
                       <div style="float:left"><input name="imageLink" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $imageLink?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelImageURLS?></label></div>
                       <div style="float:left"><input name="imageURL" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $imageURL?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelWidthS?></label></div>
                       <div style="float:left"><input name="imageWidth" type="text" size="16" maxlength="32" style="font-size:14px" value="<?php echo $imageWidth?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><?php echo $channelHeightS?></label></div>
                       <div style="float:left"><input name="imageHeight" type="text" size="16" maxlength="32" style="font-size:14px" value="<?php echo $imageHeight?>" /></div>
                       <br class="clearFloat" />
                       <div class="userFormLabel"><label><strong>&nbsp;</strong></label></div>
                       <div style="float:left">
	                    <?php
	                    	$this->DisplayFormFooter ( $doneS ); 
	                    ?>
                      </div>
                    <br class="clearFloat" />
					</div>
				</div>
			</div>
      </div>
      
	<div class="someGTitleBox">XML options</div>
		<div class="someGBox">
			<div style="float:left;width:250px;">
				<h4 style="padding:0;margin:0;">Relation to Structure</h4>
				<?php
	        	$this->DisplayFormHeadr ( 'relatestructure' ); 
	        	$this->DisplayHidden($linkName, $currentID);
	            
				$manuList = new MenuList();
				$manuListArr = $manuList->GetStructure();
				
				$structure = $rss->GetStruct();
				if ( $structure )
				{
					$structure = unserialize($structure);
				}
				else 
				{
					$structure = array();
				}
				?>
				<select name="struct[]" multiple size="10" style="width:230px;">
					<option value="1"<?php echo (in_array(1, $structure) ? " selected": "");?>>root</option>
					<?php
					for ( $i=0; $i<count($manuListArr); $i++ )
					{
						?>
						<option value="<?php echo $manuListArr[$i]["ID"];?>" disabled><?php echo $manuListArr[$i]["name"];?></option>
						<?php
						$manu = new Menu($manuListArr[$i]["ID"]);
						$manu->GetStructureFromDB($manuArr, 1, $this->m_displayLang);
						for ( $j=0; $j<count($manuArr); $j++ )
						{
							?>
							<option value="<?php echo $manuArr[$j]["ID"];?>"<?php echo (in_array($manuArr[$j]["ID"], $structure) ? " selected": "");?> style="padding-left:10px;"><?php echo $manuArr[$j]["name"];?></option>
							<?php
							$ref1 = $manuArr[$j]["ref"];
							if ( is_array($ref1) )
							{
								for ( $r1=0; $r1<count($ref1); $r1++ )
								{
									?>
									<option value="<?php echo $ref1[$r1]["ID"];?>"<?php echo (in_array($ref1[$r1]["ID"], $structure) ? " selected": "");?> style="padding-left:20px;"><?php echo $ref1[$r1]["name"];?></option>
									<?php
									$ref2 = $ref1[$r1]["ref"];
									if ( is_array($ref2) )
									{
										for ( $r2=0; $r2<count($ref2); $r2++ )
										{
											?>
											<option value="<?php echo $ref2[$r2]["ID"];?>"<?php echo (in_array($ref2[$r2]["ID"], $structure) ? " selected": "");?> style="padding-left:30px;"><?php echo $ref2[$r2]["name"];?></option>
											<?php
											$ref3 = $ref2[$r2]["ref"];
											if ( is_array($ref3) )
											{
												for ( $r3=0; $r3<count($ref3); $r3++ )
												{
													?>
													<option value="<?php echo $ref3[$r3]["ID"];?>"<?php echo (in_array($ref3[$r3]["ID"], $structure) ? " selected": "");?> style="padding-left:40px;"><?php echo $ref3[$r3]["name"];?></option>
													<?php
													$ref4 = $ref3[$r3]["ref"];
													if ( is_array($ref4) )
													{
														for ( $r4=0; $r4<count($ref4); $r4++ )
														{
															?>
															<option value="<?php echo $ref4[$r4]["ID"];?>"<?php echo (in_array($ref4[$r4]["ID"], $structure) ? " selected": "");?> style="padding-left:50px;"><?php echo $ref4[$r4]["name"];?></option>
															<?php
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					?>
				</select>
			</div>
			<div style="float:left;width:250px;">
				<h4 style="padding:0;margin:0;">Relation to Articles</h4>
				<?php
	        	$this->DisplayFormHeadr ( 'relateproducts' ); 
	        	$this->DisplayHidden($linkName, $currentID);
	            
				$PrTypeList = new Pr_typeList();
				$PrTypeListArr = $PrTypeList->GetStructure();
				
				$relations = $rss->GetRelations();
				if ( $relations )
				{
					$relations = unserialize($relations);
				}
				else 
				{
					$relations = array();
				}
				?>
				<select name="prtype[]" multiple size="10">
					<option value="1"<?php echo (in_array(1, $relations) ? " selected": "");?>>All</option>
					<?php
					for ( $i=0; $i<count($PrTypeListArr); $i++ )
					{
						?>
						<option value="<?php echo $PrTypeListArr[$i]["ID"];?>"<?php echo (in_array($PrTypeListArr[$i]["ID"], $relations) ? " selected": "");?>><?php echo $PrTypeListArr[$i]["name"];?></option>
						<?php
					}
					?>
				</select>
			</div>
			<div style="float:left;">
				<h4 style="padding:0;margin:0;">Relation type</h4>
				<input type="radio" name="relationtype" value="1"<?php echo ($rss->GetType()==1 ? " checked": "");?>> last <input type="text" name="articlecount" value="<?php echo ($rss->GetType()==1 ? $rss->GetValue(): "");?>" style="width:50px;"> articles
				<br />
				<input type="radio" name="relationtype" value="2"<?php echo ($rss->GetType()==2 ? " checked": "");?>> last <input type="text" name="daycount" value="<?php echo ($rss->GetType()==2 ? $rss->GetValue(): "");?>" style="width:50px;"> days
				<!--<br /><br />
				<h4 style="padding:0;margin:0;">Update type</h4>
				<input type="radio" name="updatetype" value="1"<?php echo ($rss->GetUType()==1 ? " checked": "");?>> auto
				<br />
				<input type="radio" name="updatetype" value="2"<?php echo ($rss->GetUType()==2 ? " checked": "");?>> manual-->
			</div>
			
			<br clear="all">
			<br clear="all">
			<input type="submit" name="generate" value="Generate">
			<input type="submit" name="donegenerate" value="Done & Generate">
			<?php
            $this->DisplayFormFooter ( $doneS ); 
            ?>
		</div>
	</div>
      
      <div class="someGTitleBox"><?php echo $itemsS?></div>
      <div class="someGBox">
      	<?php
      	// display item forms
			$items = $rss->GetItem ();
			if ($items)
			{
				// get names
				$guidS = $this->GetNameString('guid'); 
				$pubDateS = $this->GetNameString('pubDate');
				
				$index = 0;
				foreach ( $items as $item )
				{ 
					$this->DisplayFormHeadr ( 'changeItem' );
					$this->DisplayHidden ( 'itemID', $index ++ );
			      	?>
			        <div class="someWBox">
			                <div class="userFormLabel"><label><strong><?php echo $channelTitleS?></strong></label></div>
			                <div style="float:left"><input name="title" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $item['title']?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><strong><?php echo $channelLinkS?></strong></label></div>
			                <div style="float:left"><input name="link" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $item['link']?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><strong><?php echo $channelDescriptionS?></strong></label></div>
			                <div style="float:left"><textarea name="description" cols="48	" rows="3" style="font-size:14px"><?php echo $item['description']?></textarea></div>
			                <br class="clearFloat" /><br />
			                
			                <div class="userFormLabel"><label><?php echo $guidS?></label></div>
			                <div style="float:left"><input name="guid" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $item['guid']?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><?php echo $pubDateS?></label></div>
			                <div style="float:left"><input name="pubDate" type="text" size="36" maxlength="128" style="font-size:14px" value="<?php echo $item['pubDate']?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><?php echo $channelImageTitleS?></label></div>
			                <div style="float:left"><input name="imageTitle" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $item['image']->title?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><?php echo $channelImageLinkS?></label></div>
			                <div style="float:left"><input name="imageLink" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $item['image']->link?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><?php echo $channelImageURLS?></label></div>
			                <div style="float:left"><input name="imageURL" type="text" size="48" maxlength="256" style="font-size:14px" value="<?php echo $item['image']->url?>"/></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><?php echo $channelWidthS?></label></div>
			                <div style="float:left"><input name="imageWidth" type="text" size="16" maxlength="32" style="font-size:14px" value="<?php echo $item['image']->width?>" /></div>
			                <br class="clearFloat" />
			                
			                <div class="userFormLabel"><label><?php echo $channelHeightS?></label></div>
			                <div style="float:left"><input name="imageHeight" type="text" size="16" maxlength="32" style="font-size:14px" value="<?php echo $item['image']->height?>" /></div>
			                <br class="clearFloat" />
			                            
			                <div class="userFormLabel"><label><strong>&nbsp;</strong></label></div>
			                <div style="float:left">
			                	<input name="done" type="submit" value="<?php echo $doneS?>" />
			                	<input name="delete" type="submit" value="<?php echo $deleteS?>" />
			                </div>
			                <br class="clearFloat" />
			            </form>
			        </div>
			        <br />
			        <?php
				}
			}
			
			print('<div>');
			$this->DisplayFormHeadr ( 'addItem' );
			$this->DisplayFormFooter ( $newItemS );
			print('</div>');
        ?>
      </div>
      <script type="text/javascript">
			<!--
			var CollapsiblePanel1 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel1", {contentIsOpen:false});
			var CollapsiblePanel2 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel2", {contentIsOpen:false});
			var CollapsiblePanel3 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel3", {contentIsOpen:false});
			//-->
	  </script>
		<?php 
		}
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		$rss = new RSS ( 'rss/'.$this->m_fileName );
		$rss->Parse ();
		
		$res = null;
		
		switch ($this->GetFormAction ())
		{
			case 'delete':
				$res = $rss->Delete();
				header("location:index.php?RSSListID=");
				exit();
				break;
			case 'publish':
				$rss->Save(true);
				break;
			case 'changeInfo':
				// get all posts
				$title = $_POST ['title'];
				$link = $_POST['link'];
				$copyright = $_POST ['copyright'];
				$category = $_POST ['category'];
				$description = $_POST ['description'];
				$language = $_POST ['language'];
				$lastBuildDate = $_POST ['lastBuildDate'];
				$ttl = $_POST ['ttl'];
				
				$imageTitle = $_POST['imageTitle'];
				$imageLink = $_POST['imageLink'];
				$imageURL = $_POST['imageURL'];
				$imageWidth = $_POST['imageWidth'];
				$imageHeight = $_POST['imageHeight'];
				
				// update values
				$rss->SetTitle ( $title );
				$rss->SetLink($link);
				$rss->SetCopyright ( $copyright );
				$rss->SetCategory ( $category );
				$rss->SetDescription ( $description );
				$rss->SetLanguage ( $language );
				$rss->SetLastBuildDate ( $lastBuildDate );
				$rss->SetTtl ( $ttl );
				
				$rss->SetImageTitle($imageTitle);
				$rss->SetImageLink($imageLink);
				$rss->SetImageURL($imageURL);
				$rss->SetImageWidth($imageWidth);
				$rss->SetImageHeight($imageHeight);
				
				$rss->Save ();
				break;
			case 'addItem':
				// get new item
				$newItem = array ();
				$newItem ['pubDate'] = date(DATE_RSS);
				// add new item
				$rss->AddNewItem ( $newItem );
				$rss->Save ();
				break;
			case 'changeItem':
				if (isset ( $_POST ['delete'] ))
				{
					$itemID = $_POST ['itemID'];
					$rss->DeleteItem ( $itemID );
					$rss->Save ();
				}
				elseif (isset ( $_POST ['done'] ))
				{
					$itemID = $_POST ['itemID'];
					$newItem = array ();
					
					$newItem ['title'] = $_POST ['title'];
					$newItem ['link'] = $_POST ['link'];
					$newItem ['guid'] = $_POST ['guid'];
					$newItem ['pubDate'] = $_POST ['pubDate'];
					$newItem ['description'] = $_POST ['description'];
					
					$newItem['image']['title'] = $_POST['imageTitle'];
					$newItem['image']['link'] = $_POST['imageLink'];
					$newItem['image']['url'] = $_POST['imageURL'];
					$newItem['image']['width'] = $_POST['imageWidth'];
					$newItem['image']['height'] = $_POST['imageHeight'];
					
					$rss->UpdateItem($itemID, $newItem);
					$rss->Save ();
				}
				break;
			case 'relateproducts':
				if ( isset($_POST["generate"]) )
				{
					$rss->Generate($this->m_site->GetSiteURL(), $this->m_displayLang);
				}
				elseif ( isset($_POST["donegenerate"]) )
				{
					//	edit features
					$struct = DB::POST("struct");
					if ( $struct )
					{
						$struct = serialize($struct);
					}
					$rss->SetStruct($struct);
					
					$prtype = DB::POST("prtype");
					if ( $prtype )
					{
						$prtype = serialize($prtype);
					}
					$rss->SetRelations($prtype);
					
					$relationtype = DB::POST("relationtype");
					$rss->SetType($relationtype);
					
					$updatetype = DB::POST("updatetype");
					$rss->SetUType($updatetype);
					
					$count = $relationtype==1 ? DB::POST("articlecount") : DB::POST("daycount");
					$rss->SetValue($count);
					
					//	generate
					$rss->Generate($this->m_site->GetSiteURL(), $this->m_displayLang);
				}
				else 
				{
					$struct = DB::POST("struct");
					if ( $struct )
					{
						$struct = serialize($struct);
					}
					$rss->SetStruct($struct);
					
					$prtype = DB::POST("prtype");
					if ( $prtype )
					{
						$prtype = serialize($prtype);
					}
					$rss->SetRelations($prtype);
					
					$relationtype = DB::POST("relationtype");
					$rss->SetType($relationtype);
					
					$count = $relationtype==1 ? DB::POST("articlecount") : DB::POST("daycount");
					$rss->SetValue($count);
					
					$updatetype = DB::POST("updatetype");
					$rss->SetUType($updatetype);
				}
				
				break;
		}
		
		return $res;
	}
}
?>