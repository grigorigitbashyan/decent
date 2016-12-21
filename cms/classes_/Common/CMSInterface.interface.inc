<?php
 /**
 * This interface is used for all CMS classes.
 * It contains 3 main fnctions to display menu and features, 
 * also DoAction which is palse to do actions before any display.
 *
 * @package Main
 * @author Vazgen Manukyan
 * 
 */
interface CMSInterface
{
    /**
     * This function is called after any output of content.
     *
     */
    function DoAction();
    
    /**
     * This function is used to display any information on a left site.
     *
     */
    function DisplayMenu();
    
    /**
     * This function is used to display any information on a center.
     *
     */
    function DisplayFeatures();
}
?>