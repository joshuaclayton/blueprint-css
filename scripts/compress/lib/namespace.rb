#
# HTML classes namespace adder
# This class is used to add namespaces to the test files,
# so that the tests will work even if you set your own namespace.
#

class Namespace
  
  # Read html to string, remove namespace if any,
  # set the new namespace, and update the test file.
  def initialize(path, namespace)
    html = File.path_to_string(path)
    remove_current_namespace(html)
    add_namespace(html, namespace)
    File.string_to_file(path, html)
  end
  
  # adds namespace to BP classes in a html file
  def add_namespace(html, namespace)    
    html.gsub!(/(class=")([a-zA-Z0-9\-_ ]*)(")/) { |m|
      classes = m.to_s.split('"')[1].split(' ')
      classes.map! { |c| c = namespace + c }
      'class="' + classes.join(' ') + '"'
    }
    html
  end
  
  # removes a namespace from a string of html
  def remove_current_namespace(html)
    current = current_namespace(html)
    html.gsub!(current, '')
    html
  end
  
  # returns current namespace in test files
  # based on container class
  def current_namespace(html)
    html =~ /class="([\S]+)container/
    current_namespace = $1 if $1
    return current_namespace || ''
  end

end