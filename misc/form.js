// $Id: form.js,v 1.4 2009-03-08 01:43:57 webchick Exp $
(function($) {

Drupal.behaviors.multiselectSelector = {
  attach: function(context) {
    // Automatically selects the right radio button in a multiselect control.
    $('.multiselect select:not(.multiselectSelector-processed)', context)
      .addClass('multiselectSelector-processed').change(function() {
        $('.multiselect input:radio[value="'+ this.id.substr(5) +'"]')
          .attr('checked', true);
    });
  }
};


/**
 * Automatically displays the guidelines of the selected text format.
 */
Drupal.behaviors.filterGuidelines = {
  attach: function(context) {
  $('.filter-guidelines:not(.filterGuidelines-processed)', context)
      .addClass('filterGuidelines-processed')
      .find('label').hide()
      .parents('.filter-wrapper').find('select.filter-list')
      .bind('change', function() {
        $(this).parents('.filter-wrapper')
        .find('.filter-guidelines-item').hide()
        .siblings('#filter-guidelines-' + this.value).show();
      })
      .change();
  }
};

})(jQuery);