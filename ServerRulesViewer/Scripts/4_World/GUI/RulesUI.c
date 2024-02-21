class SRV_RulesUI: UIScriptedMenu
{
	protected const string m_LayoutPath 	= "ServerRulesViewer/ServerRulesViewer/GUI/layouts/SRV_RulesUI.layout";
	protected const string m_IconHeartRed 	= "set:srv_ui image:srv_heart_red";
	protected const string m_IconHeartWhite = "set:srv_ui image:srv_heart_white";
			
	protected const int m_CategoriesPerPage = 5;	
	protected int m_CategoriesPageIndex;
	protected int m_CategoriesMaxPages;
	
	protected GridSpacerWidget m_CategoriesGrid;
	protected GridSpacerWidget m_RulesGrid;

	protected ButtonWidget m_ExitBtn;
	protected ButtonWidget m_DonateBtn;
	
	protected TextWidget m_TitleTW;
	protected TextWidget m_ExitBtnTW;
	protected TextWidget m_CategoryPageHelperTW;
	
	protected ImageWidget m_DonateIW;
	
	protected Widget m_RuleContentPW;
	
	protected ref array<ref SRV_CategoryWidget> m_CategoryWidgets;
	protected ref array<ref SRV_RuleWidget> m_RuleWidgets;
	protected ref SRV_Config m_Config;
	
	protected SRV_CategoryWidget m_SelectedCategory;
	
	override Widget Init()
	{
		m_Config 				= GetSRVConfigManagerPlugin().GetConfig();
        layoutRoot	 			= GetGame().GetWorkspace().CreateWidgets(m_LayoutPath);
		
		m_CategoriesGrid 		= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("categoriesGrid"));
		m_RulesGrid 			= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("rulesGrid"));
		
        m_ExitBtn 				= ButtonWidget.Cast(layoutRoot.FindAnyWidget("exitBtn"));
        m_DonateBtn 			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("donateBtn"));
		
        m_TitleTW 				= TextWidget.Cast(layoutRoot.FindAnyWidget("titleTW"));
        m_ExitBtnTW 			= TextWidget.Cast(layoutRoot.FindAnyWidget("exitBtnTW"));
        //m_CategoryPageHelperTW 	= TextWidget.Cast(layoutRoot.FindAnyWidget("categoryPageHelperTW"));
		
		m_DonateIW				= ImageWidget.Cast(layoutRoot.FindAnyWidget("donateBtnIW"));
		
		m_RuleContentPW 		= Widget.Cast(layoutRoot.FindAnyWidget("ruleContentPW"));
		
		m_CategoryWidgets 		= new array<ref SRV_CategoryWidget>();
		m_RuleWidgets 			= new array<ref SRV_RuleWidget>();

		m_CategoriesMaxPages 	= Math.Floor(m_Config.categories.Count() / m_CategoriesPerPage);
		
		m_DonateIW.LoadImageFile(0, m_IconHeartWhite);
		m_TitleTW.SetText(m_Config.title);
		m_RuleContentPW.Show(false);
		
		DisplayCategories(m_Config.categories);

		Print("Init SRV_RulesUI");
				
		return layoutRoot;
	}
	
	void ~SRV_RulesUI()
    {     
		m_CategoryWidgets.Clear();  
    }

	override void OnShow()
    {
        super.OnShow();
        GetGame().GetUIManager().ShowCursor(true);
        GetGame().GetMission().GetHud().Show(false);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        PPEffects.SetBlurMenu(0.3);
    }

    override void OnHide()
    {
        super.OnHide();
		GetGame().GetMission().PlayerControlEnable(true);
        GetGame().GetUIManager().ShowCursor(false);
        GetGame().GetMission().GetHud().Show(true);
        GetGame().GetUIManager().Back();
        PPEffects.SetBlurMenu(0);
    }
	
	override void Update( float timeslice )
	{
		super.Update(timeslice);
		
		if (GetGame() && GetGame().GetInput() && GetGame().GetInput().LocalPress("UAUIBack", false))
		{
			OnHide();
		}
		
		int posx, posy;
		GetMousePos(posx, posy);
		
		if (SRV_Util.IsInsideWidget(m_ExitBtn, posx, posy))
		{
			m_ExitBtnTW.SetColor(COLOR_RED);
		}
		else
		{
			m_ExitBtnTW.SetColor(COLOR_WHITE);
		}
		
		if (SRV_Util.IsInsideWidget(m_DonateBtn, posx, posy))
		{
			m_DonateIW.LoadImageFile(0, m_IconHeartRed);
		}
		else
		{
			m_DonateIW.LoadImageFile(0, m_IconHeartWhite);
		}
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (SRV_Util.IsInsideWidget(m_CategoriesGrid, x, y))
		{
			if (wheel == 1 && m_CategoriesPageIndex > 0)
			{
				m_CategoriesPageIndex = Math.Clamp(--m_CategoriesPageIndex, 0, m_CategoriesMaxPages);
				DisplayCategories(m_Config.categories);
			}
					
			if (wheel == -1 && m_CategoriesPageIndex < m_CategoriesMaxPages)
			{
				m_CategoriesPageIndex = Math.Clamp(++m_CategoriesPageIndex, 0, m_CategoriesMaxPages);
				DisplayCategories(m_Config.categories);
			}
		}
		
		return super.OnMouseWheel(w, x, y, wheel);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{		
		if (w == m_ExitBtn) 
		{
			Print("[ServerRulesViewer] m_ExitBtn clicked");
			OnHide();
			
			return true;
		}
		
		if (w == m_DonateBtn) 
		{
			Print("[ServerRulesViewer] m_DonateBtn clicked");
			
			if (m_Config.donateLink != "")
			{
				Print(m_Config.donateLink);
				GetGame().OpenURL(m_Config.donateLink);
			}
			
			return true;
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	void DisplayCategories(array<ref SRV_Category> categories)
	{
		m_CategoryPageHelperTW.SetText(string.Format("Page %1/%2", m_CategoriesPageIndex + 1, m_CategoriesMaxPages + 1));
		
		int startIndex = m_CategoriesPageIndex * m_CategoriesPerPage;
		int endIndex = Math.Min(startIndex + m_CategoriesPerPage, m_Config.categories.Count());

		m_CategoryWidgets.Clear();
		for (int i = startIndex; i < endIndex; i++)
		{
			SRV_Category category = categories[i];
			m_CategoryWidgets.Insert(new SRV_CategoryWidget(m_CategoriesGrid, this, category));
		}
		
		if (m_SelectedCategory)
		{
			for (int j = 0; j < m_CategoryWidgets.Count(); j++)
			{
				SRV_CategoryWidget srvCategoryWidget = SRV_CategoryWidget.Cast(m_CategoryWidgets[j]);
				if (srvCategoryWidget.GetRuleCategory().category == m_SelectedCategory.GetRuleCategory().category)
					srvCategoryWidget.SetActive(true);
			}
		}
	}
	
	void DisplayRules(TStringArray rules)
	{
		m_RuleWidgets.Clear();
		for (int i = 0; i < rules.Count(); i++)
		{
			m_RuleWidgets.Insert(new SRV_RuleWidget(m_RulesGrid, this, i + 1, rules[i]));
			//m_RulesLBW.AddItem(string.Format("%1", i + 1), rules, 0, i);
			//m_RulesLBW.SetItem(i, rules[i], rules, 1);
		}
	}
	
	void OnClickCategory(SRV_CategoryWidget categoryWidget)
	{	
		if (m_SelectedCategory)
			m_SelectedCategory.SetActive(!m_SelectedCategory.IsActive());
		
		if (categoryWidget == m_SelectedCategory)
		{
			m_SelectedCategory = null;
			m_RuleContentPW.Show(false);
		}
		else
		{
			m_SelectedCategory = categoryWidget;
			m_RuleContentPW.Show(true);
			DisplayRules(categoryWidget.GetRuleCategory().rules);
		}
	}
}

class SRV_CategoryWidget extends ScriptedWidgetEventHandler 
{	
	const string m_LayoutPath = "ServerRulesViewer/ServerRulesViewer/GUI/layouts/SRV_Category.layout";
	protected SRV_RulesUI m_ParentUI;
	protected SRV_Category m_Category;
	protected bool m_IsActive;
	
	protected Widget m_Root;
	protected GridSpacerWidget m_ParentWidget;
	protected ButtonWidget m_CategoryBtn;
	protected Widget m_CategoryBtnBgPW;
	protected TextWidget m_CategoryBtnTW;
	
	void SRV_CategoryWidget(GridSpacerWidget parentWidget, SRV_RulesUI ui, SRV_Category category)
	{		
		m_ParentWidget 		 		= GridSpacerWidget.Cast(parentWidget);
		m_ParentUI 			 		= SRV_RulesUI.Cast(ui);
		m_Category			 		= category;
		
		m_Root				 		= GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, m_ParentWidget);
		m_CategoryBtn				= ButtonWidget.Cast(m_Root.FindAnyWidget("categoryBtn"));
		m_CategoryBtnBgPW			= Widget.Cast(m_Root.FindAnyWidget("categoryBtnBgPW"));
		m_CategoryBtnTW				= TextWidget.Cast(m_Root.FindAnyWidget("categoryBtnTW"));
		
		m_CategoryBtnTW.SetText(m_Category.category);
		
		m_Root.SetHandler(this);
		m_Root.Show(true);
	}
		
	void ~SRV_CategoryWidget()
	{
		Print("SRV_CategoryWidget deleted");
		m_ParentWidget.RemoveChild(m_Root);
	}
	
	bool IsActive()
	{
		return m_IsActive;
	}
	
	SRV_Category GetRuleCategory()
	{
		return m_Category;
	}
	
	void SetActive(bool value)
	{
		m_IsActive = value;
		
		if (m_IsActive)
			m_CategoryBtnBgPW.SetAlpha(1);
		else
			m_CategoryBtnBgPW.SetAlpha(0);
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		SetActive(true);
		m_ParentUI.OnClickCategory(this);
		
		return super.OnClick(w, x, y, button);
	}
}

class SRV_RuleWidget extends ScriptedWidgetEventHandler 
{	
	const string m_LayoutPath = "ServerRulesViewer/ServerRulesViewer/GUI/layouts/SRV_Rule.layout";
	protected SRV_RulesUI m_ParentUI;
	
	protected Widget m_Root;
	protected GridSpacerWidget m_ParentWidget;
	protected MultilineTextWidget m_RuleMLTW;
	protected TextWidget m_RuleNumberTW;
	
	void SRV_RuleWidget(GridSpacerWidget parentWidget, SRV_RulesUI ui, int number, string rule)
	{		
		m_ParentWidget 		 		= GridSpacerWidget.Cast(parentWidget);
		m_ParentUI 			 		= SRV_RulesUI.Cast(ui);
		
		m_Root				 		= GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, m_ParentWidget);
		m_RuleMLTW					= MultilineTextWidget.Cast(m_Root.FindAnyWidget("ruleMLTW"));
		m_RuleNumberTW				= TextWidget.Cast(m_Root.FindAnyWidget("ruleNumberTW"));
		
		m_RuleMLTW.SetText(rule);
		m_RuleNumberTW.SetText(number.ToString());
		
		m_Root.SetHandler(this);
		m_Root.Show(true);
	}
		
	void ~SRV_RuleWidget()
	{
		Print("SRV_RuleWidget deleted");
		m_ParentWidget.RemoveChild(m_Root);
	}
}